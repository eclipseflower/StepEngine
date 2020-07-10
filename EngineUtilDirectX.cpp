#include "EngineUtilDirectX.h"

Engine::DxException::DxException(HRESULT hr, const wstring & functionName, const wstring & filename, int lineNumber) :
	ErrorCode(hr),
	FunctionName(functionName),
	Filename(filename),
	LineNumber(lineNumber)
{

}