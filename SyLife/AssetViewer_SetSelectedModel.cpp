#include "AssetViewer.h"

#include "PartPaletteViewer.h"
#include "Cell.h"
#include "Molecule.h"
#include "AssemblyViewer.h"
#include "Body.h"
#include "Equipment.h"
#include "Module.h"
#include "PartShapeViewer.h"

void AssetViewer::SetSelectedModel(const shared_ptr<Model>& model)
{
	// ModelViewer������
	g_viewerManagerPtr->DeleteViewer<ModelViewer>();

	// �Ώۂ�View��ǉ�
	model->MakeViewer();

	// �Ώۃ��f���̐ݒ�
	m_selectedModel = model;
}