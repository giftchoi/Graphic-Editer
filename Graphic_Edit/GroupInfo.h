#pragma once
#include "Group.h"
// GroupInfo command target

class GroupInfo : public CObject
{
	DECLARE_SERIAL(GroupInfo)
public:
	GroupInfo();
	virtual ~GroupInfo();

	CPtrList* getGroupItemList(int Groupidx);
	CRgn* GroupRgn(int Groupidx);
	void GroupDelete(int Groupidx);
	int GroupCreate(CPtrList* itemList);
	void getGroupsID(CPtrList* itemList, int idArray[], int* idUsed);
	CRect* getRect(int GroupIdx);
	CRgn* getRgn(int GroupIdx);
	void setRectAndRgn(int GroupIdx);
	CPtrList* getGroupList() { return &m_groupList; }					//그룹의 리스트
	void GroupItemDelete(CPtrList* itemList);
	void resize(int GroupIdx, int dx, int dy);
	Group* getGroup(int idx);
private:
	CPtrList m_groupList;					//그룹의 리스트

	int getTopGroupID(int ID);
public:
	virtual void Serialize(CArchive& ar);
};


