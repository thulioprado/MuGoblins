#include "Library.h"
#include "Model.h"
#include "Item.h"
#include "Sign.h"

CModel::CModel()
{
}

CModel::~CModel()
{
}

void CModel::Load()
{
	Memory::Call(0x60ABBB, this->LoadModels);
}

ModelInfo* CModel::Get(int Model) const
{
	static ModelInfo* Models = (ModelInfo*)(MODEL_BASE);
	return &Models[Model];
}

void CModel::LoadModels()
{
	pLoadModels();

	Item.LoadModels();
	Sign.LoadModels();
}

CModel Model;