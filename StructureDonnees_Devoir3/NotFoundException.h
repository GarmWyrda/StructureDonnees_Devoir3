#pragma once
#include <exception>
#include "vcruntime_exception.h"
class NotFoundException :
	public std::exception
{
public:
	NotFoundException();
	virtual ~NotFoundException();
};