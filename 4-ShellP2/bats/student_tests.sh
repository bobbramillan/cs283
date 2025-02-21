#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF
    # Assertions
    [ "$status" -eq 0 ]
}

@test "empty input shows warning" {
    run ./dsh <<EOF

EOF
    [ "$status" -eq 0 ]
    [[ "$output" =~ "$CMD_WARN_NO_CMD" ]]
}

@test "leading and trailing spaces are removed" {
    run ./dsh <<EOF
     echo Hello    
EOF
    [ "$status" -eq 0 ]
    [[ "$output" =~ "Hello" ]]
}

@test "multiple spaces between arguments are reduced" {
    run ./dsh <<EOF
echo   multiple    spaces   test
EOF
    [ "$status" -eq 0 ]
    [[ "$output" =~ "multiple spaces test" ]]
}

@test "quoted strings are preserved" {
    run ./dsh <<EOF
echo "hello    world"
EOF
    [ "$status" -eq 0 ]
    [[ "$output" =~ "hello    world" ]]
}

@test "quoted strings with unecessary spaces are preserved" {
    run ./dsh <<EOF
echo "  hello world  "
EOF
    [ "$status" -eq 0 ]
    [[ "$output" =~ "  hello world  " ]]
}

@test "cd with no arguments does nothing" {
    current_dir=$(pwd)
    run ./dsh <<EOF
cd
pwd
EOF
    [ "$status" -eq 0 ]
    [[ "$output" =~ "$current_dir" ]]
}

@test "cd to valid directory works" {
    mkdir -p testdir
    run ./dsh <<EOF
cd testdir
pwd
EOF
    [ "$status" -eq 0 ]
    [[ "$output" =~ "$PWD/testdir" ]]
    rmdir testdir
}

@test "cd to non-existent directory fails" {
    run ./dsh <<EOF
cd /this/directory/does/not/exist
EOF
    [ "$status" -ne 0 ]
    [[ "$output" =~ "error: could not change directory" ]]
}

@test "cd with too many arguments fails" {
    run ./dsh <<EOF
cd /tmp extra_argument
EOF
    [ "$status" -ne 0 ]
    [[ "$output" =~ "error: too many arguments for cd" ]]
}

@test "exit command terminates the shell" {
    run ./dsh <<EOF
exit
EOF
    [ "$status" -eq 0 ]
}

@test "ls executes successfully" {
    run ./dsh <<EOF
ls
EOF
    [ "$status" -eq 0 ]
}

@test "pwd returns current directory" {
    run ./dsh <<EOF
pwd
EOF
    [ "$status" -eq 0 ]
    [ -n "$output" ]
}

@test "echo with no arguments prints a newline" {
    run ./dsh <<EOF
echo
EOF
    [ "$status" -eq 0 ]
}

@test "ls with arguments (-l) executes successfully" {
    run ./dsh <<EOF
ls -l
EOF
    [ "$status" -eq 0 ]
}

@test "invalid command fails" {
    run ./dsh <<EOF
invalidcommand
EOF
    [ "$status" -ne 0 ]
    [[ "$output" =~ "$CMD_ERR_EXECUTE" ]]
}

@test "command with multiple arguments executes correctly" {
    run ./dsh <<EOF
ls -l /tmp
EOF
    [ "$status" -eq 0 ]
}

@test "fork failure returns ERR_MEMORY" {
    run ./dsh <<EOF
ulimit -u 1 # Artificially limit processes
ls
EOF
    [ "$status" -ne 0 ]
    [[ "$output" =~ "error: fork failed" ]]
}