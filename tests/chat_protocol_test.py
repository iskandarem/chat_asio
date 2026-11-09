#!/usr/bin/env python3

import argparse
import socket
import subprocess
import sys
import time


HOST = "127.0.0.1"
IO_TIMEOUT_SECONDS = 3.0


def reserve_port() -> int:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.bind((HOST, 0))
        return sock.getsockname()[1]


def connect_when_ready(port: int, server: subprocess.Popen[str]) -> socket.socket:
    deadline = time.monotonic() + IO_TIMEOUT_SECONDS

    while time.monotonic() < deadline:
        if server.poll() is not None:
            stdout, stderr = server.communicate()
            raise RuntimeError(
                f"server exited during startup with status {server.returncode}\n"
                f"stdout:\n{stdout}\nstderr:\n{stderr}"
            )

        try:
            connection = socket.create_connection((HOST, port), timeout=0.2)
            connection.settimeout(IO_TIMEOUT_SECONDS)
            return connection
        except OSError:
            time.sleep(0.02)

    raise TimeoutError("server did not start listening in time")


def receive_line(connection: socket.socket) -> bytes:
    response = bytearray()

    while not response.endswith(b"\n"):
        chunk = connection.recv(4096)
        if not chunk:
            raise RuntimeError(f"connection closed before a complete line: {response!r}")
        response.extend(chunk)

    return bytes(response)


def assert_echo(connection: socket.socket, message: bytes) -> None:
    connection.sendall(message + b"\n")
    expected = b"echo: " + message + b"\n"
    actual = receive_line(connection)

    if actual != expected:
        raise AssertionError(f"expected {expected!r}, received {actual!r}")


def stop_server(server: subprocess.Popen[str]) -> tuple[str, str]:
    if server.poll() is None:
        server.terminate()

    try:
        return server.communicate(timeout=2)
    except subprocess.TimeoutExpired:
        server.kill()
        return server.communicate(timeout=2)


def run_test(server_executable: str, client_executable: str) -> None:
    port = reserve_port()
    server = subprocess.Popen(
        [server_executable, HOST, str(port)],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )

    server_stdout = ""
    server_stderr = ""

    try:
        # Leave one session waiting for its newline. An asynchronous server must
        # still accept and answer the next client while this one is incomplete.
        incomplete_client = connect_when_ready(port, server)
        with incomplete_client:
            incomplete_client.sendall(b"not finished")

            with connect_when_ready(port, server) as framed_client:
                assert_echo(framed_client, b"second client")
                assert_echo(framed_client, b"another message")

            client = subprocess.run(
                [client_executable, HOST, str(port)],
                input="hello from client\n",
                capture_output=True,
                text=True,
                timeout=IO_TIMEOUT_SECONDS,
                check=False,
            )

            if client.returncode != 0:
                raise AssertionError(
                    f"client exited with status {client.returncode}\n"
                    f"stdout:\n{client.stdout}\nstderr:\n{client.stderr}"
                )

            expected_output = "Server: echo: hello from client"
            if expected_output not in client.stdout:
                raise AssertionError(
                    f"client output does not contain {expected_output!r}\n"
                    f"stdout:\n{client.stdout}\nstderr:\n{client.stderr}"
                )
    finally:
        server_stdout, server_stderr = stop_server(server)

    if server.returncode not in (-15, -9):
        raise AssertionError(
            f"server exited unexpectedly with status {server.returncode}\n"
            f"stdout:\n{server_stdout}\nstderr:\n{server_stderr}"
        )


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--server", required=True)
    parser.add_argument("--client", required=True)
    args = parser.parse_args()

    try:
        run_test(args.server, args.client)
    except (AssertionError, OSError, RuntimeError, subprocess.TimeoutExpired) as error:
        print(f"FAIL: {error}", file=sys.stderr)
        return 1

    print("PASS: asynchronous newline echo protocol")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
