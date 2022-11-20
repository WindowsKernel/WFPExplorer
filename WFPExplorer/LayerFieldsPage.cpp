#include "pch.h"
#include "LayerFieldsPage.h"
#include <WFPEngine.h>
#include "StringHelper.h"
#include <SortHelper.h>

CLayerFieldsPage::CLayerFieldsPage(WFPEngine& engine, WFPLayerInfo& layer) : m_Engine(engine), m_Layer(layer) {
}

CString CLayerFieldsPage::GetColumnText(HWND, int row, int col) const {
	auto& field = m_Fields[row];

	switch (col) {
		case 0: return StringHelper::WFPConditionFieldKeyToString(field.FieldKey);
		case 1: return StringHelper::WFPFieldTypeToString(field.Type);
		case 2: return StringHelper::WFPDataTypeToString(field.DataType);
	}
	return CString();
}

void CLayerFieldsPage::DoSort(SortInfo const* si) {
	auto compare = [&](auto& f1, auto& f2) {
		switch (si->SortColumn) {
			case 0: return SortHelper::Sort(StringHelper::WFPConditionFieldKeyToString(f1.FieldKey), StringHelper::WFPConditionFieldKeyToString(f2.FieldKey), si->SortAscending);
			case 1: return SortHelper::Sort(f1.Type, f2.Type, si->SortAscending);
			case 2: return SortHelper::Sort(StringHelper::WFPDataTypeToString(f1.DataType), StringHelper::WFPDataTypeToString(f2.DataType), si->SortAscending);
		}
		return false;
	};
	std::ranges::sort(m_Fields, compare);
}

LRESULT CLayerFieldsPage::OnInitDialog(UINT, WPARAM, LPARAM, BOOL&) {
	InitDynamicLayout(false);
	m_List.Attach(GetDlgItem(IDC_LIST));
	m_List.SetExtendedListViewStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT);

	m_Fields = m_Engine.GetLayerByKey(m_Layer.LayerKey)->Fields;
	m_List.SetItemCount(m_Layer.NumFields);

	auto cm = GetColumnManager(m_List);
	cm->AddColumn(L"Field", 0, 230);
	cm->AddColumn(L"Type", 0, 80);
	cm->AddColumn(L"Data Type", 0, 150);

	return 0;
}