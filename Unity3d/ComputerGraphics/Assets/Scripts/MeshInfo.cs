/*##################################
����Ƽ �޽� ����Ȯ�� ��ũ��Ʈ(������)
���ϸ�: MeshInfo.cs
�ۼ���: ��ȫ�� (downkhg@gmail.com)
������������¥: 2024.01.11
����: 1.0
###################################*/
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MeshInfo : MonoBehaviour
{
    public int idx = 0; //������Ʈ�� ��ȣ�� �ִ´�.
    MeshFilter m_meshFilter;
    Mesh m_mesh;
    int m_nIndicesNumber = 0;

    // Use this for initialization
    void Start()
    {
        //�޽������� ���Ե� ���ӿ�����Ʈ�� ������Ʈ���� �����´�.
        m_meshFilter = GetComponent<MeshFilter>();
        m_mesh = m_meshFilter.mesh;
    }

    private void OnGUI()
    {
        //�޽÷������� �ﰢ�������� �����ϴ� GUI
        GUI.BeginGroup(new Rect(150 * idx, 0, 150, 120));
        if (GUI.Button(new Rect(0, 0, 50, 20), "-"))
        {
            m_nIndicesNumber -= 3;
        }
        GUI.Box(new Rect(50, 0, 50, 20), "" + m_nIndicesNumber);
        if (GUI.Button(new Rect(100, 0, 50, 20), "+"))
        {
            m_nIndicesNumber += 3;
        }
        string msg = string.Format("vertex:{0}\n,Indices:{1}\n", m_mesh.vertexCount, m_mesh.subMeshCount);
        for (int i = 0; i < m_mesh.subMeshCount; i++)
        {
            msg += string.Format("{0},", m_mesh.GetIndices(i).Length);
        }
        GUI.Box(new Rect(0, 20, 150, 100), msg);
        GUI.EndGroup();
    }

    private void OnDrawGizmos()
    {
        //��ķκ��� ��ġ,ȸ��,ũ�� ������ �����´�.
        Vector3 pos = transform.position;
        Quaternion quaternion = transform.localRotation;
        Vector3 scale = transform.localScale;

        for (int i = 0; i < m_mesh.subMeshCount; i++)
        {
            //�ﰢ�� ������ �޾ƿ´�.
            int[] indices = m_mesh.GetIndices(i);

            int a = indices[m_nIndicesNumber + 0];
            int b = indices[m_nIndicesNumber + 1];
            int c = indices[m_nIndicesNumber + 2];

            Vector3 vA = m_mesh.vertices[a];
            Vector3 vB = m_mesh.vertices[b];
            Vector3 vC = m_mesh.vertices[c];
            //�� �ﰢ���� ���ʹϾ��� ���Ͽ� ��Ŀ� ���� ���ؽ��� ��ġ�� ��ȯ�Ѵ�.
            vA = quaternion * vA;
            vB = quaternion * vB;
            vC = quaternion * vC;
            //�� �ﰢ���� ��ġ�� �̵���Ų��.
            vA += pos;
            vB += pos;
            vC += pos;

            Gizmos.color = Color.red;
            //�� �ﰢ������ ���� �׸���.
            Gizmos.DrawLine(vA, vB);
            Gizmos.DrawLine(vA, vC);
            Gizmos.DrawLine(vB, vC);

            //�� ��������� ������ ����� �����.
            Plane plane = new Plane(vA, vB, vC);
            //��鿡 �븻�� �׸���.
            Gizmos.DrawLine(pos, pos + plane.normal);
        }
    }

    // Update is called once per frame
    void Update()
    {

    }
}
