/*##################################
유니티 메시 정보확인 스크립트(수업용)
파일명: MeshInfo.cs
작성자: 김홍규 (downkhg@gmail.com)
마지막수정날짜: 2024.01.11
버전: 1.0
###################################*/
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MeshInfo : MonoBehaviour
{
    public int idx = 0; //오브젝트의 번호를 넣는다.
    MeshFilter m_meshFilter;
    Mesh m_mesh;
    int m_nIndicesNumber = 0;

    // Use this for initialization
    void Start()
    {
        //메시정보가 포함된 게임오브젝트의 컴포넌트에서 가져온다.
        m_meshFilter = GetComponent<MeshFilter>();
        m_mesh = m_meshFilter.mesh;
    }

    private void OnGUI()
    {
        //메시랜더러에 삼각형정보를 조작하는 GUI
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
        //행렬로부터 위치,회전,크기 정보를 가져온다.
        Vector3 pos = transform.position;
        Quaternion quaternion = transform.localRotation;
        Vector3 scale = transform.localScale;

        for (int i = 0; i < m_mesh.subMeshCount; i++)
        {
            //삼각형 정보를 받아온다.
            int[] indices = m_mesh.GetIndices(i);

            int a = indices[m_nIndicesNumber + 0];
            int b = indices[m_nIndicesNumber + 1];
            int c = indices[m_nIndicesNumber + 2];

            Vector3 vA = m_mesh.vertices[a];
            Vector3 vB = m_mesh.vertices[b];
            Vector3 vC = m_mesh.vertices[c];
            //각 삼각형에 쿼터니언을 곱하여 행렬에 따라 버텍스의 위치를 변환한다.
            vA = quaternion * vA;
            vB = quaternion * vB;
            vC = quaternion * vC;
            //각 삼각형의 위치를 이동시킨다.
            vA += pos;
            vB += pos;
            vC += pos;

            Gizmos.color = Color.red;
            //각 삼각형으로 선을 그린다.
            Gizmos.DrawLine(vA, vB);
            Gizmos.DrawLine(vA, vC);
            Gizmos.DrawLine(vB, vC);

            //각 삼격형으로 가상의 평면을 만든다.
            Plane plane = new Plane(vA, vB, vC);
            //평면에 노말을 그린다.
            Gizmos.DrawLine(pos, pos + plane.normal);
        }
    }

    // Update is called once per frame
    void Update()
    {

    }
}
