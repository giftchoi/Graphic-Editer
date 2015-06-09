// GroupInfo.cpp : implementation file
//

#include "stdafx.h"
#include "Graphic_Edit.h"
#include "GroupInfo.h"


// GroupInfo

GroupInfo::GroupInfo()
{
	m_groupList.RemoveAll();
}

GroupInfo::~GroupInfo()
{
}


// GroupInfo member functions


void GroupInfo::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		m_groupList.Serialize(ar);
	}
	else
	{	// loading code
		m_groupList.Serialize(ar);
	}
}

CPtrList* GroupInfo::getGroupItemList(int Groupidx)
{
	Group* G;

	//전체를 돌면서 ID랑 같은 그룹이 있는지 찾아 본후 있으면 그 그룹을 바로 리턴
	POSITION pos = m_groupList.GetHeadPosition();

	while (pos)
	{
		G = (Group*)m_groupList.GetNext(pos);			// element 가져와서 
		if (G->getID() == Groupidx)
		{
			return G->getItemList();
		}
	}

	return NULL;
}

CRgn* GroupInfo::GroupRgn(int Groupidx)
{
	Group* G;

	//전체를 돌면서 ID랑 같은 그룹이 있는지 찾아 본후 있으면 그 그룹을 바로 리턴
	POSITION pos = m_groupList.GetHeadPosition();

	while (pos)
	{
		G = (Group*)m_groupList.GetNext(pos);			// element 가져와서 
		if (G->getID() == Groupidx)
		{
			return G->getRgn();
		}
	}

	return NULL;

}

void GroupInfo::GroupDelete(int Groupidx)
{
	Group* G;

	POSITION pos = m_groupList.GetHeadPosition();

	while (pos)
	{
		POSITION deletePos = pos;
		G = (Group*)m_groupList.GetNext(pos);			// element 가져와서 

		if (G->getID() == Groupidx)
		{
			G->Delete();
			m_groupList.RemoveAt(deletePos);
			break;
		}
	}
}

int GroupInfo::GroupCreate(CPtrList* itemList)
{
	Group* G = new Group(itemList, &m_groupList);
	m_groupList.AddTail(G);

	return G->getID();
}

void GroupInfo::getGroupsID(CPtrList* itemList, int idArray[], int* idUsed)
{
	GObject* item;
	int tempId;

	POSITION pos = itemList->GetHeadPosition();

	while (pos)
	{
		item = (GObject*)itemList->GetNext(pos);			// element 가져와서 
		if (item->getID() == -1)
		{
			continue;
		}

		tempId = getTopGroupID(item->getID());

		bool isDuplication = false;

		for (int i = 0; i < *idUsed; i++)
		{
			if (idArray[i] == tempId)
			{
				isDuplication = true;
				break;
			}
		}

		if (!isDuplication)
		{
			idArray[*idUsed] = tempId;
			(*idUsed)++;
		}
	}
}

int GroupInfo::getTopGroupID(int ID)
{
	Group* group;

	//아이디를 가진 그룹을 찾음
	POSITION pos = m_groupList.GetHeadPosition();
	while (pos)
	{
		group = (Group*)m_groupList.GetNext(pos);

		if (group->getID() == ID)
		{
			if (group->getParentID() == -1)
			{
				return group->getID();
			}
			else
			{
				return getTopGroupID(group->getParentID());
			}
		}
	}

	return -1;

}

CRect* GroupInfo::getRect(int GroupIdx)
{
	Group* G;

	//전체를 돌면서 ID랑 같은 그룹이 있는지 찾아 본후 있으면 그 그룹을 바로 리턴
	POSITION pos = m_groupList.GetHeadPosition();

	while (pos)
	{
		G = (Group*)m_groupList.GetNext(pos);			// element 가져와서 
		if (G->getID() == GroupIdx)
		{
			return G->getRect();
		}
	}

	return NULL;
}

CRgn* GroupInfo::getRgn(int GroupIdx)
{
	Group* G;

	//전체를 돌면서 ID랑 같은 그룹이 있는지 찾아 본후 있으면 그 그룹을 바로 리턴
	POSITION pos = m_groupList.GetHeadPosition();

	while (pos)
	{
		G = (Group*)m_groupList.GetNext(pos);			// element 가져와서 
		if (G->getID() == GroupIdx)
		{
			return G->getRgn();
		}
	}

	return NULL;
}

void GroupInfo::setRectAndRgn(int GroupIdx)
{
	Group* G;

	//전체를 돌면서 ID랑 같은 그룹이 있는지 찾아 본후 있으면 그 그룹을 바로 리턴
	POSITION pos = m_groupList.GetHeadPosition();

	while (pos)
	{
		G = (Group*)m_groupList.GetNext(pos);			// element 가져와서 
		if (G->getID() == GroupIdx)
		{
			G->SetRgn();
			POSITION subPos = m_groupList.GetHeadPosition();

			while (subPos)
			{
				Group* subGroup = (Group*)m_groupList.GetNext(subPos);

				if (subGroup->getParentID() == G->getID())
				{
					setRectAndRgn(subGroup->getID());
				}
			}

			break;
		}
	}
}

void GroupInfo::GroupItemDelete(CPtrList* itemList)
{
	GObject* ogbj;
	GObject* groupOgbj;
	Group* G;

	//전체를 돌면서 ID랑 같은 그룹이 있는지 찾아 본후 있으면 그 그룹을 바로 리턴
	POSITION itemPos = itemList->GetHeadPosition();

	while (itemPos)
	{
		ogbj = (GObject*)itemList->GetNext(itemPos);			// element 가져와서 

		POSITION groupPos = m_groupList.GetHeadPosition();
		while (groupPos)
		{
			G = (Group*)m_groupList.GetNext(groupPos);			// element 가져와서 

			CPtrList* groupItemList = G->getItemList();

			POSITION groupItemPos = groupItemList->GetHeadPosition();

			while (groupItemPos)
			{
				POSITION nowPos = groupItemPos;				//삭제하기 위해서 보관
				groupOgbj = (GObject*)groupItemList->GetNext(groupItemPos);			// element 가져와서 
				if (groupOgbj == ogbj)
				{
					groupItemList->RemoveAt(nowPos);
				}
			}
		}
	}

	//아이템이 하나도 없는 그룹은 삭제되어야 함.. 그러므로 삭제
	POSITION groupPos = m_groupList.GetHeadPosition();
	while (groupPos)
	{
		G = (Group*)m_groupList.GetNext(groupPos);			// element 가져와서 
		CPtrList* groupItemList = G->getItemList();

		if (groupItemList->IsEmpty())
		{
			GroupDelete(G->getID());
		}
	}
}

void GroupInfo::resize(int GroupIdx, int dx, int dy)
{
	Group* G;
	POSITION groupPos = m_groupList.GetHeadPosition();
	while (groupPos)
	{
		G = (Group*)m_groupList.GetNext(groupPos);			// element 가져와서 
		if (G->getID() == GroupIdx)
		{
			CPtrList* gobjList = G->getItemList();
			POSITION objPos = gobjList->GetHeadPosition();

			while (objPos)
			{
				GObject* gobj = (GObject*)gobjList->GetHeadPosition();
				if (gobj->getObjectType() == RECTANGLE || gobj->getObjectType() == CIRCLE)
				{
					gobj->move(dx, dy);
				}
			}

			break;
		}
	}
}

Group* GroupInfo::getGroup(int idx)
{
	Group* G;
	POSITION groupPos = m_groupList.GetHeadPosition();
	while (groupPos)
	{
		G = (Group*)m_groupList.GetNext(groupPos);			// element 가져와서 
		if (G->getID() == idx)
		{
			return G;
		}
	}

	return NULL;
}

IMPLEMENT_SERIAL(GroupInfo, GObject, 1);

