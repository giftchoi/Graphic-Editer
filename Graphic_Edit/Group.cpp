// Group.cpp : implementation file
//

#include "stdafx.h"
#include "Graphic_Edit.h"
#include "Group.h"


// Group

Group::Group(CPtrList* ItemList, CPtrList* groupList)
{
	m_selectPointIdx = -1;

	m_groupList = groupList;
	int childID[100];
	int childIDUsed = 0;

	m_ID = IDNumber++;
	m_Parent_ID = -1;

	POSITION pos = ItemList->GetHeadPosition();

	while (pos)			// ���� ���õ� element �鿡 ����
	{
		GObject* item = (GObject*)ItemList->GetNext(pos);			// element �����ͼ� 
		m_groupItem.AddTail(item);								// grouplist �� �߰�

		if (item->getID() == -1)									//���� -1�̸� ���� �׷��� ������.
		{
			item->setID(m_ID);
		}
		else													//���� �׷��� �־�����
		{
			//���� �׷��� �ֻ����� �θ� ã�ư�
			Group* group = (Group*)getGroup(item->getID());

			while (group->getParentID() != -1)
			{
				group = (Group*)getGroup(group->getParentID());
			}

			//�ֻ����� �θ�� �׷��� ���� ���� �� �ۿ� ����
			//���̵� childID�� �ִ� �۾��� ��(�ߺ������ؼ� ����)
			bool isDuplication = false;

			for (int i = 0; i < childIDUsed; i++)
			{
				if (childID[i] == group->getID())
				{
					isDuplication = true;
					break;
				}
			}

			if (!isDuplication)
			{
				childID[childIDUsed++] = group->getID();
				/*
				childID[childIDUsed++] = item->getID();
				*/
			}
		}
	}

	//�ֻ��� �θ���� ParentID�� ���θ���� ���� �׷��� ���̵� �־���
	for (int i = 0; i < childIDUsed; i++)
	{
		Group* group = (Group*)getGroup(childID[i]);
		group->setParentID(m_ID);
	}

	SetRgn();
}

Group::~Group()
{
}
// Group member functions
void Group::Delete()
{
	int ID_set[100];
	int ID_set_used = 0;
	POSITION pos;

	//�ڽ��� �ִ��� ��� �˾ƿ�
	ID_set_used = getChildIdSet(ID_set);


	for (int i = 0; i < ID_set_used; i++)
	{
		Group* subgroup = (Group*)getGroup(ID_set[i]);
		subgroup->setParentID(-1);
	}

	//���� �ڽ��� ���� ��ü�� �ƾƵ� ���ڰ� �ִٸ� �׷��� ��������̹Ƿ� ��� -1�� �ʱ�ȭ ����� �� 
	pos = m_groupItem.GetHeadPosition();
	while (pos)
	{
		GObject* item = (GObject *)m_groupItem.GetNext(pos);	// element �����ͼ�

		if (item->getID() == m_ID)
		{
			item->setID(-1);
		}
	}
}


int Group::getChildIdSet(int ID_set[100])
{
	int ID_set_used = 0;
	int cur_Group_ID = -1;
	POSITION pos;

	pos = m_groupList->GetTailPosition();

	while (pos)
	{
		Group * group = (Group *)m_groupList->GetPrev(pos);	// element �����ͼ� 

		//�������� �θ� ������ ���� ���̵��϶����� ã��
		if (group->getParentID() == m_ID)
		{
			bool isDuplication = false;

			for (int i = 0; i < ID_set_used; i++)
			{
				if (ID_set[i] == group->getParentID())
				{
					isDuplication = true;
					break;
				}
			}

			if (!isDuplication)
			{
				ID_set[ID_set_used] = group->getID();
				ID_set_used++;
			}
		}
	}




	/*
	//�׷� ��ü�� ���鼭
	pos = m_groupItem.GetHeadPosition();
	while(pos)
	{
	GObject * item = (GObject *)m_groupItem.GetNext(pos);	// element �����ͼ�
	cur_item_ID = item->getID();

	//�������� ������ �ִ� ���̵� �����ͼ� �������� ���� �ִ� �׷��� ã��
	GGroup* subgroup = (GGroup*)getGroup(cur_item_ID);

	//�������� �θ� ������ ���� ���̵��϶����� ã��
	while(subgroup->getParentID() != m_ID && subgroup->getParentID() != -1)
	{
	cur_item_ID = subgroup->getParentID();
	subgroup = (GGroup*)getGroup(cur_item_ID);
	}

	//�ߺ��� ���� �ϰ� ����
	bool isDuplication = false;

	for(int i = 0; i < ID_set_used; i++)
	{
	if(ID_set[i] == subgroup->getID())
	{
	isDuplication = true;
	break;
	}
	}

	if(!isDuplication)
	{
	ID_set[ID_set_used] = subgroup->getID();
	ID_set_used++;
	}
	}
	*/

	//������
	return ID_set_used;
}

Group* Group::getGroup(int groupID)
{
	Group* group;

	//��ü�� ���鼭 ID�� ���� �׷��� �ִ��� ã�� ���� ������ �� �׷��� �ٷ� ����
	POSITION pos = m_groupList->GetTailPosition();
	while (pos)												// ���� ���õ� element �鿡 ����
	{
		group = (Group*)m_groupList->GetPrev(pos);			// element �����ͼ� 

		if (groupID == group->getID())
		{
			return group;
		}
	}

	return NULL;
}

void Group::SetRgn()
{
	//��ü�� ���鼭 ID�� ���� �׷��� �ִ��� ã�� ���� ������ �� �׷��� �ٷ� ����
	GObject* item;
	CPoint RSPoint(99999, 99999);
	CPoint REPoint(-99999, -99999);

	POSITION pos = m_groupItem.GetHeadPosition();
	while (pos)												// ���� ���õ� element �鿡 ����
	{
		item = (GObject*)m_groupItem.GetNext(pos);			// element �����ͼ� 

		CPoint SPoint = item->getPoint();
		CPoint EPoint = item->getPointEnd();

		if (RSPoint.x > SPoint.x)
		{
			RSPoint.x = SPoint.x;
		}

		if (REPoint.x < EPoint.x)
		{
			REPoint.x = EPoint.x;
		}

		if (RSPoint.y > SPoint.y)
		{
			RSPoint.y = SPoint.y;
		}

		if (REPoint.y < EPoint.y)
		{
			REPoint.y = EPoint.y;
		}
	}

	m_Rect.TopLeft().x = RSPoint.x - 10;
	m_Rect.TopLeft().y = RSPoint.y - 10;

	m_Rect.BottomRight().x = REPoint.x + 10;
	m_Rect.BottomRight().y = REPoint.y + 10;

	m_Rgn.DeleteObject();
	m_Rgn.CreateRectRgn(RSPoint.x - 20, RSPoint.y - 20, REPoint.x + 20, REPoint.y + 20);

}

void Group::DrawPoint(CDC* pdc)
{
	CPoint tSpoint;
	CPoint tEpoint;
	tSpoint.x = m_Rect.left;
	tSpoint.y = m_Rect.top;
	tEpoint.x = m_Rect.right;
	tEpoint.y = m_Rect.bottom;

	CRect rect(tSpoint, tEpoint);

	selectRect[0].SetRect(tSpoint.x - 5, tSpoint.y - 5,
		tSpoint.x + 5, tSpoint.y + 5);

	selectRect[1].SetRect(rect.CenterPoint().x - 5, tSpoint.y - 5,
		rect.CenterPoint().x + 5, tSpoint.y + 5);

	selectRect[2].SetRect(tEpoint.x - 5, tSpoint.y - 5,
		tEpoint.x + 5, tSpoint.y + 5);


	selectRect[3].SetRect(tEpoint.x - 5, rect.CenterPoint().y - 5,
		tEpoint.x + 5, rect.CenterPoint().y + 5);


	selectRect[4].SetRect(tEpoint.x - 5, tEpoint.y - 5,
		tEpoint.x + 5, tEpoint.y + 5);

	selectRect[5].SetRect(rect.CenterPoint().x - 5, tEpoint.y - 5,
		rect.CenterPoint().x + 5, tEpoint.y + 5);

	selectRect[6].SetRect(tSpoint.x - 5, tEpoint.y - 5,
		tSpoint.x + 5, tEpoint.y + 5);


	selectRect[7].SetRect(tSpoint.x - 5, rect.CenterPoint().y - 5,
		tSpoint.x + 5, rect.CenterPoint().y + 5);

	for (int i = 0; i < 8; i++)
	{
		CPen pen(PS_SOLID, 1, RGB(0, 0, 255));
		pdc->SelectObject(pen);
		pdc->SelectStockObject(WHITE_BRUSH);
		pdc->Ellipse(selectRect[i]);
	}
}

void Group::selectPoint(CPoint point)
{
	m_selectPointIdx = -1;

	for (int i = 0; i < 8; i++)
	{
		if (selectRect[i].PtInRect(point))
		{
			m_selectPointIdx = i;
			break;
		}
	}
}

int Group::IDNumber = 0;

IMPLEMENT_SERIAL(Group, GObject, 1);

void Group::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		m_groupItem.Serialize(ar);
		m_Rgn.Serialize(ar);
		m_groupList->Serialize(ar);										//���� �� �غ��߰ڴ�

		ar << IDNumber << m_Parent_ID << m_Rect;
	}
	else
	{	// loading code
		m_groupItem.Serialize(ar);
		m_Rgn.Serialize(ar);
		m_groupList->Serialize(ar);

		ar >> IDNumber >> m_Parent_ID >> m_Rect;
	}
}

