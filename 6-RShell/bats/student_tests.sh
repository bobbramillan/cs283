#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

setup()

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "basic command execution: ls" {
    run ./dsh -c "ls"
    [ "$status" -eq 0 ]
    [ "${#output}" -gt 0 ]
}

@test "check command execution: echo" {
    run ./dsh -c "echo Hello, World!"
    [ "$status" -eq 0 ]
    [ "$output" = "Hello, World!" ]
}

@test "check pipeline: ls | grep .c" {
    run ./dsh -c "ls | grep .c"
    [ "$status" -eq 0 ]
    [[ "$output" == *".c"* ]]
}

@test "check if pipeline: echo Hello | wc -w" {
    run ./dsh -c "echo Hello World | wc -w"
    [ "$status" -eq 0 ]
    [ "$output" = "2" ]
}

@test "Built-in command: cd and pwd" {
    run ./dsh -c "cd /tmp && pwd"
    [ "$status" -eq 0 ]
    [[ "$output" == "/tmp" ]]
}

@test "Built-in command: exit" {
    run ./dsh -c "exit"
    [ "$status" -eq 0 ]
}

@test "Invalid Command" {
    run ./dsh -c "someinvalidcommand"
    [ "$status" -eq 1 ]
}

@test "Too many pipes" {
    run ./dsh -c "echo 1 | echo 2 | echo 3 | echo 4 | echo 5 | echo 6 | echo 7 | echo 8 | echo 9"
    [ "$status" -eq 1 ]
    [[ "$output" == *"error: piping limited"* ]]
}

@test "Empty command should not crash" {
    run ./dsh -c ""
    [ "$status" -eq 0 ]
}

@test "Redirection: echo Hello > test_output.txt" {
    run ./dsh -c "echo Hello > test_output.txt"
    [ "$status" -eq 0 ]
    run cat test_output.txt
    [ "$output" = "Hello" ]
}

@test "Append redirection: echo Again >> test_output.txt" {
    run ./dsh -c "echo Again >> test_output.txt"
    [ "$status" -eq 0 ]
    run cat test_output.txt
    [[ "$output" == *"Hello"* ]]
    [[ "$output" == *"Again"* ]]
}
