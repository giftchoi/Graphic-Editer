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

	//��ü�� ���鼭 ID�� ���� �׷��� �ִ��� ã�� ���� ������ �� �׷��� �ٷ� ����
	POSITION pos = m_groupList.GetHeadPosition();

	while (pos)
	{
		G = (Group*)m_groupList.GetNext(pos);			// element �����ͼ� 
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

	//��ü�� ���鼭 ID�� ���� �׷��� �ִ��� ã�� ���� ������ �� �׷��� �ٷ� ����
	POSITION pos = m_groupList.GetHeadPosition();

	while (pos)
	{
		G = (Group*)m_groupList.GetNext(pos);			// element �����ͼ� 
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
		G = (Group*)m_groupList.GetNext(pos);			// element �����ͼ� 

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
		item = (GObject*)itemList->GetNext(pos);			// element �����ͼ� 
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

	//���̵� ���� �׷��� ã��
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

	//��ü�� ���鼭 ID�� ���� �׷��� �ִ��� ã�� ���� ������ �� �׷��� �ٷ� ����
	POSITION pos = m_groupList.GetHeadPosition();

	while (pos)
	{
		G = (Group*)m_groupList.GetNext(pos);			// element �����ͼ� 
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

	//��ü�� ���鼭 ID�� ���� �׷��� �ִ��� ã�� ���� ������ �� �׷��� �ٷ� ����
	POSITION pos = m_groupList.GetHeadPosition();

	while (pos)
	{
		G = (Group*)m_groupList.GetNext(pos);			// element �����ͼ� 
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

	//��ü�� ���鼭 ID�� ���� �׷��� �ִ��� ã�� ���� ������ �� �׷��� �ٷ� ����
	POSITION pos = m_groupList.GetHeadPosition();

	while (pos)
	{
		G = (Group*)m_groupList.GetNext(pos);			// element �����ͼ� 
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

	//��ü�� ���鼭 ID�� ���� �׷��� �ִ��� ã�� ���� ������ �� �׷��� �ٷ� ����
	POSITION itemPos = itemList->GetHeadPosition();

	while (itemPos)
	{
		ogbj = (GObject*)itemList->GetNext(itemPos);			// element �����ͼ� 

		POSITION groupPos = m_groupList.GetHeadPosition();
		while (groupPos)
		{
			G = (Group*)m_groupList.GetNext(groupPos);			// element �����ͼ� 

			CPtrList* groupItemList = G->getItemList();

			POSITION groupItemPos = groupItemList->GetHeadPosition();

			while (groupItemPos)
			{
				POSITION nowPos = groupItemPos;				//�����ϱ� ���ؼ� ����
				groupOgbj = (GObject*)groupItemList->GetNext(groupItemPos);			// element �����ͼ� 
				if (groupOgbj == ogbj)
				{
					groupItemList->RemoveAt(nowPos);
				}
			}
		}
	}

	//�������� �ϳ��� ���� �׷��� �����Ǿ�� ��.. �׷��Ƿ� ����
	POSITION groupPos = m_groupList.GetHeadPosition();
	while (groupPos)
	{
		G = (Group*)m_groupList.GetNext(groupPos);			// element �����ͼ� 
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
		G = (Group*)m_groupList.GetNext(groupPos);			// element �����ͼ� 
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
		G = (Group*)m_groupList.GetNext(groupPos);			// element �����ͼ� 
		if (G->getID() == idx)
		{
			return G;
		}
	}

	return NULL;
}

IMPLEMENT_SERIAL(GroupInfo, GObject, 1);

