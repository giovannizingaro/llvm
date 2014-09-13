#pragma once
#include <cstdlib>
#include <iostream>
#include "TaggedData.h"
#include "CalcDFG.h"
#include "DFGPrinter.h"

namespace
{
	static struct NCFAForcePassLinking {
		NCFAForcePassLinking() {
			// We must reference the passes in such a way that compilers will not
			// delete it all as dead code, even with whole program optimization,
			// yet is effectively a NO-OP. As the compiler isn't smart enough
			// to know that getenv() never returns -1, this will do the job.
			if (std::getenv("bar") != (char*) - 1)
			{ return; }
			std::cout << "init" << std::endl;
			(void) llvm::createTaggedDataPass();
			(void) llvm::createDFGPrinterPass();
		}
	} NCFAForcePassLinking; // Force link by creating a global definition.
}