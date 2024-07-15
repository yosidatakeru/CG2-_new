#include"engine/base/DirectXCommon.h"
class ModelCommon
{
public:
	void Initialze(DirectXCommon* directXCommon);

	DirectXCommon* GetDirectXCmmon() const { return directXCommon_; }
private:
	DirectXCommon* directXCommon_;

};
