#include "EngineUtilDirectX.h"

Engine::DxException::DxException(HRESULT hr, const wstring & functionName, const wstring & filename, int lineNumber) :
	ErrorCode(hr),
	FunctionName(functionName),
	Filename(filename),
	LineNumber(lineNumber)
{

}

wstring Engine::DxException::ToString() const
{
	// Get the string description of the error code.
	_com_error err(ErrorCode);
	std::string msg = err.ErrorMessage();

	return FunctionName + L" failed in " + Filename + L"; line " + std::to_wstring(LineNumber) + L"; error: " + AnsiToWString(msg);
}
