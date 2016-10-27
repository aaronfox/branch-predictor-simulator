#pragma once

#define NOT_BRANCH 0
#define BRANCH 1

#define NOT_TAKEN 0
#define TAKEN 1

#define STRONGLY_TAKEN 3
#define WEAKLY_TAKEN 2
#define WEAKLY_NOT_TAKEN 1
#define STRONGLY_NOT_TAKEN 0

#define VALID 1
#define INVALID 0

#define CORRECT 0
#define WRONG 1

#define _error_exit(fun) { perror(fun); exit(EXIT_FAILURE); }

#define _output_error_exit(fun) { printf(fun); exit(EXIT_FAILURE);}

#define pow_2(num) ((uint32_t)(1 << ((int)num)))

typedef char uint8_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef enum Branch_Result
{ 
	not_branch = NOT_BRANCH,
	branch = BRANCH 
} Branch_Result;

typedef enum Taken_Result
{ 
	not_taken = NOT_TAKEN,
	taken = TAKEN 
} Taken_Result;

typedef struct Result
{
	Branch_Result predict_branch;
	Branch_Result predict_taken;
	Taken_Result actual_branch;
	Taken_Result actual_taken;
}Result;

uint32_t Get_Index(uint32_t addr, uint32_t index_width);