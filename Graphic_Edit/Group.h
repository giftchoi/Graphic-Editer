#pragma once
#include "GObject.h"

// Group command target

class Group : public GObject
{
	DECLARE_SERIAL(Group)
public:
	Group(){}
	Group(CPtrList* ItemList, CPtrList* groupList);
	virtual ~Group();

	int getParentID(){ return m_Parent_ID; }
	void setParentID(int parentID){ m_Parent_ID = parentID; }
	int getChildIdSet(int* ID_set);
	Group* getGroup(int groupID);
	CPtrList* getItemList() { return &m_groupItem; }
	void Delete();
	void SetRgn();
	CRgn* getRgn() { return &m_Rgn; }
	void setRect() { SetRgn(); }
	CRect* getRect() { return &m_Rect; }
	BOOL pointInRgn(CPoint point) { return m_Rgn.PtInRegion(point); }
	void DrawPoint(CDC* pdc);
	void selectPoint(CPoint point);
	int getSelectPoint() { return m_selectPointIdx; }

private:
	CPtrList m_groupItem;
	CPtrList* m_groupList;
	static int IDNumber;
	int m_ID;
	int m_Parent_ID;

	CRgn m_Rgn;
	CRect m_Rect;

	int m_selectPointIdx;
	CRect selectRect[8];
public:
	
	virtual void Serialize(CArchive& ar);
};


