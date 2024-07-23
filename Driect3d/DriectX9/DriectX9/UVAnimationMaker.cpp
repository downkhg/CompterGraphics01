#include <iostream>
#include <list>
#include <vector>
#include <string>

using namespace std;

struct TexCoords
{
	float u;
	float v;
	TexCoords(float u = 0, float v = 0)
	{
		this->u = u;
		this->v = v;
	}
	void Print()
	{
		printf("(%f,%f)\n",this->u,this->v);
	}
};

void WriteFileText(int width, int hight)
{
	int nSellWidth = width;
	int nSellHighit = hight;
	int nSellSize = nSellWidth * nSellHighit;
	TexCoords sSize(1.0f / (float)nSellWidth, 1.0f / (float)nSellHighit);
	TexCoords sTL(0, 0), sTR(sSize.u, 0), sBL(0, sSize.v), sBR = sSize;
	TexCoords arrRectInit[4] = { TexCoords(0,0),TexCoords(sSize.u,0), TexCoords(0,sSize.v), sSize };
	TexCoords arrRect[4] = { TexCoords(0,0),TexCoords(sSize.u,0), TexCoords(0,sSize.v), sSize };
	vector<vector<vector<TexCoords>>> arrTextureCoords(nSellHighit);
	FILE* pFile = fopen("texCoords.txt", "wt");
	fprintf(pFile, "%d %d\n", nSellWidth, nSellHighit);
	for (int y = 0; y < nSellHighit; y++)
	{
		arrTextureCoords[y].resize(nSellWidth);
		for (int x = 0; x < nSellWidth; x++)
		{
			arrTextureCoords[y][x].resize(4);
			for (int i = 0; i < 4; i++)
			{
				arrTextureCoords[y][x][i] = arrRect[i];
				printf("[%d,%d][%d]:", y, x, i);
				arrTextureCoords[y][x][i].Print();
				arrRect[i].u += sSize.u;
				fprintf(pFile, "%f %f ", arrTextureCoords[y][x][i].u, arrTextureCoords[y][x][i].v);
			}
			fprintf(pFile, "\n");
		}
		for (int i = 0; i < 4; i++)
		{
			arrRect[i].u = arrRectInit[i].u;
			arrRect[i].v += sSize.v;
		}
	}
	fclose(pFile);
}

void ReadFileText()
{
	int nSellWidth;
	int nSellHighit;
	vector<vector<vector<TexCoords>>> arrTextureCoords(nSellHighit);
	FILE* pFile = fopen("texCoords.txt", "rt");
	fscanf(pFile, "%d %d\n", &nSellWidth, &nSellHighit);
	
	arrTextureCoords.resize(nSellHighit);
	for (int y = 0; y < nSellHighit; y++)
	{
		arrTextureCoords[y].resize(nSellWidth);
		for (int x = 0; x < nSellWidth; x++)
		{
			arrTextureCoords[y][x].resize(4);
			for (int i = 0; i < 4; i++)
			{
				fscanf(pFile, "%f %f ", &arrTextureCoords[y][x][i].u, &arrTextureCoords[y][x][i].v);
				printf("%f %f ", arrTextureCoords[y][x][i].u, arrTextureCoords[y][x][i].v);
			}
			printf("\n");
		}
	}
	fclose(pFile);
}

void main()
{
	ReadFileText();
}