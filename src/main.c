#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "btb.h"
#include "bp.h"

#ifdef DBG
FILE *debug_fp;
#endif

BTB *branch_target_buffer;
BP *branch_predictor;
Stat stat;
char *trace_file;

int main(int argc, char *argv[])
{
#ifdef DBG
	debug_fp = fopen("debug.txt", "w");
	if (debug_fp == NULL)
		_error_exit("fopen")
#endif


	Predictor type;
	uint32_t width[9];
	parse_arguments(argc, argv, &type, width);

	branch_target_buffer = NULL;
	branch_predictor = NULL;
	if (width[BTBuffer] != 0 && width[ASSOC] != 0)
	{
		branch_target_buffer = (BTB *)malloc(sizeof(BTB));
		if (branch_target_buffer == NULL)
			_error_exit("malloc")
		BTB_Init(width[ASSOC], width[BTBuffer]);
	}

	branch_predictor = (BP *)malloc(sizeof(BP));
	if (branch_predictor == NULL)
		_error_exit("malloc")

	Predictor_Init(type, width);

	Stat_Init();

	FILE *trace_file_fp = fopen(trace_file, "r");
	if (trace_file_fp == NULL)
		_error_exit("fopen")

	uint64_t trace_count = 0;
	while (1)
	{
#ifdef DBG
		fprintf(debug_fp, "--------------\n\n");
#endif
		/* read the trace */
		uint8_t take_or_not, line;
		uint32_t addr;
		int rr = fscanf(trace_file_fp, "%x %c%c", &addr, &take_or_not, &line);
		trace_count++;
		if (rr == EOF)
			break;

		/* make branch prediction */
		Result result = Predictor_Predict(addr);
		if (branch_target_buffer == NULL)
			result.predict_branch = branch;
		else
			result.predict_branch = BTB_Predict(addr);

		result.actual_branch = branch;
		if (take_or_not == 't')
			result.actual_taken = taken;
		else
			result.actual_taken = not_taken;

		/* update the predictor and statistic data */
		Update_Stat(result);
		if (result.predict_branch == branch)
			Predictor_Update(addr, result);
		if (branch_target_buffer != NULL)
			BTB_Update(addr, result, trace_count);

	}

	FILE *fp = stdout;
	Result_fprintf(fp, argc, argv);
#ifdef DBG
	Result_fprintf(debug_fp, argc, argv);
#endif
}
