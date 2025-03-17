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

@test "Check if echo works" {
    run ./dsh <<EOF
echo "Hello, World!"
EOF
    [ "$status" -eq 0 ]
    [ "$output" = "Hello, World!" ]
}

@test "Check if invalid command fails" {
    run ./dsh <<EOF
invalidcommand
EOF
    [ "$status" -ne 0 ]
}

@test "Pipeline: ls | wc -l" {
    run ./dsh <<EOF
ls | wc -l
EOF
    [ "$status" -eq 0 ]
}

@test "Pipeline: echo test | grep t" {
    run ./dsh <<EOF
echo "test" | grep "t"
EOF
    [ "$status" -eq 0 ]
    [ "$output" = "test" ]
}

@test "Output redirection: echo test > out.txt" {
    run ./dsh <<EOF
echo "test" > out.txt
EOF
    [ "$status" -eq 0 ]
}

@test "Input redirection: cat < out.txt" {
    run ./dsh <<EOF
cat < out.txt
EOF
    [ "$status" -eq 0 ]
    [ "$output" = "test" ]
}

@test "Check empty input" {
    run ./dsh <<EOF

EOF
    [ "$status" -eq 0 ]
}

@test "Check invalid syntax: ls || grep .c" {
    run ./dsh <<EOF
ls || grep .c
EOF
    [ "$status" -ne 0 ]
}

@test "Check too many pipes (expect failure)" {
    run ./dsh <<EOF
ls | grep .c | sort | uniq | wc | cat | head | tail | tr | awk | sed
EOF
    [ "$status" -ne 0 ]
}
