#include "pch.h"
#include "WFPHelper.h"
#include <WFPEngine.h>
#include "StringHelper.h"
#include "LayerGeneralPage.h"
#include "LayerFieldsPage.h"
#include "FilterGeneralPage.h"
#include "FilterConditionsPage.h"
#include "LayersView.h"

CString WFPHelper::GetProviderName(WFPEngine const& engine, GUID const& key) {
	if (key != GUID_NULL) {
		auto provider = engine.GetProviderByKey(key);
		if (provider && provider->displayData.name && provider->displayData.name[0] != L'@')
			return provider->displayData.name;
		return StringHelper::GuidToString(key);
	}
	return L"";
}

CString WFPHelper::GetFilterName(WFPEngine const& engine, GUID const& key) {
	if (key != GUID_NULL) {
		auto filter = engine.GetFilterByKey(key);
		if (filter && !filter->Name.empty() && filter->Name[0] != L'@')
			return filter->Name.c_str();
		return StringHelper::GuidToString(key);
	}
	return L"";
}

CString WFPHelper::GetLayerName(WFPEngine const& engine, GUID const& key) {
	if (key != GUID_NULL) {
		auto layer = engine.GetLayerByKey(key);
		if (layer && layer->displayData.name && layer->displayData.name[0] != L'@')
			return layer->displayData.name;
		return StringHelper::GuidToString(key);
	}
	return L"";
}

CString WFPHelper::GetSublayerName(WFPEngine const& engine, GUID const& key) {
	if (key != GUID_NULL) {
		auto layer = engine.GetSublayerByKey(key);
		if (layer && !layer->Name.empty() && layer->Name[0] != L'@')
			return layer->Name.c_str();
		return StringHelper::GuidToString(key);
	}
	return L"";
}

int WFPHelper::ShowLayerProperties(WFPEngine& engine, FWPM_LAYER* layer) {
	auto name = L"Layer Properties (" + GetLayerName(engine, layer->layerKey) + L")";
	CPropertySheet sheet((PCWSTR)name);
	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW | PSH_USEICONID | PSH_NOCONTEXTHELP | PSH_RESIZABLE;
	sheet.m_psh.pszIcon = MAKEINTRESOURCE(IDI_LAYERS);
	CLayerGeneralPage general(engine, layer);
	general.m_psp.dwFlags |= PSP_USEICONID;
	general.m_psp.pszIcon = MAKEINTRESOURCE(IDI_CUBE);
	sheet.AddPage(general);

	CLayerFieldsPage fields(engine, layer);
	if (layer->numFields > 0) {
		fields.m_psp.dwFlags |= PSP_USEICONID;
		fields.m_psp.pszIcon = MAKEINTRESOURCE(IDI_FIELD);
		sheet.AddPage(fields);
	}
	return (int)sheet.DoModal();
}

int WFPHelper::ShowFilterProperties(WFPEngine& engine, WFPFilterInfo const& filter) {
	auto name = L"Filter Properties (" + GetFilterName(engine, filter.FilterKey) + L")";
	CPropertySheet sheet((PCWSTR)name);
	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW | PSH_USEICONID | PSH_NOCONTEXTHELP | PSH_RESIZABLE;
	sheet.m_psh.pszIcon = MAKEINTRESOURCE(IDI_FILTER);
	CFilterGeneralPage general(engine, filter);
	general.m_psp.dwFlags |= PSP_USEICONID;
	general.m_psp.pszIcon = MAKEINTRESOURCE(IDI_CUBE);
	CFilterConditionsPage cond(engine, filter);
	sheet.AddPage(general);
	if (filter.ConditionCount > 0) {
		cond.m_psp.dwFlags |= PSP_USEICONID;
		cond.m_psp.pszIcon = MAKEINTRESOURCE(IDI_CONDITION);
		sheet.AddPage(cond);
	}
	return (int)sheet.DoModal();
}

int WFPHelper::ShowSublayerProperties(WFPEngine& engine, WFPSubLayerInfo& layer) {
	return 0;
}

int WFPHelper::ShowProviderProperties(WFPEngine& engine, FWPM_PROVIDER* provider) {
	return 0;
}
