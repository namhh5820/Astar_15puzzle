#include <iostream>
#include <queue>
#include <vector>
#include <stdio.h>
using namespace std;
#define MAX 4
//Cau truc trang thai của puzzle.
struct STATE
{
	//Mang hai chieu luu tru ma tran puzzle
	int Taci[MAX][MAX];
	//Dong trong va cot trong de luu o so 0.
	int DongTrong;
	int CotTrong;
	//LuuViTri : so buoc chuyen
	//Previous : trang thai truoc do.
	int LuuViTri, Previous;
	//g : do sau cua trang thai.	
	int g, h;
};
void InputGraph(STATE &start, STATE &end, char *input);
int check(STATE A, STATE B);
STATE ChuyenTrai(STATE A);
STATE ChuyenPhai(STATE A);
STATE ChuyenTren(STATE A);
STATE ChuyenDuoi(STATE A);
void PrintPath(vector<STATE> &Path, char *output, int LuuViTri);
int calInversions(STATE C);
bool operator > (const STATE &A, const STATE &B);
int Heuristic(STATE A, STATE end);
int Asolver(STATE start, STATE end, vector<STATE> &Path);
void main(int argc, char* argv[])
{
	if (argc != 3)
	{
		cout << "loi tham so dong lenh \n";
	}
	else
	{
		char *input = argv[1];
		char *output = argv[2];
		int ketqua;
		STATE start, end;
		vector <STATE> Path;
		InputGraph(start, end, input);
		ketqua = Asolver(start, end, Path);
		PrintPath(Path, output, ketqua);
	}
}
void InputGraph(STATE &start, STATE &end, char *input)
{
	FILE *f;
	errno_t err;
	err = fopen_s(&f, input, "r");
	if (f != NULL)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				fscanf_s(f, "%d", &start.Taci[i][j]);
				if (start.Taci[i][j] == 0)
				{
					start.DongTrong = i;
					start.CotTrong = j;
				}
			}
		start.g = 0;
		start.LuuViTri = 0;
		start.Previous = -1;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				fscanf_s(f, "%d", &end.Taci[i][j]);
				if (end.Taci[i][j] == 0)
				{
					end.DongTrong = i;
					end.CotTrong = j;
				}
			}
		start.h = Heuristic(start, end);
	}
	else
	{
		cout << "khong the doc file \n";
		return;
	}

	fclose(f);
}
int check(STATE A, STATE B)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (A.Taci[i][j] != B.Taci[i][j])
				return 0;
	return 1;
}

STATE ChuyenTrai(STATE A)
{
	A.Taci[A.DongTrong][A.CotTrong] = A.Taci[A.DongTrong][A.CotTrong - 1];
	A.Taci[A.DongTrong][A.CotTrong - 1] = 0;
	A.CotTrong = A.CotTrong - 1;
	A.Previous = A.LuuViTri;
	return A;
}
STATE ChuyenPhai(STATE A)
{
	A.Taci[A.DongTrong][A.CotTrong] = A.Taci[A.DongTrong][A.CotTrong + 1];
	A.Taci[A.DongTrong][A.CotTrong + 1] = 0;
	A.CotTrong = A.CotTrong + 1;
	A.Previous = A.LuuViTri;
	return A;
}
STATE ChuyenTren(STATE A)
{
	A.Taci[A.DongTrong][A.CotTrong] = A.Taci[A.DongTrong - 1][A.CotTrong];
	A.Taci[A.DongTrong - 1][A.CotTrong] = 0;
	A.DongTrong = A.DongTrong - 1;
	A.Previous = A.LuuViTri;
	return A;
}
STATE ChuyenDuoi(STATE A)
{
	A.Taci[A.DongTrong][A.CotTrong] = A.Taci[A.DongTrong + 1][A.CotTrong];
	A.Taci[A.DongTrong + 1][A.CotTrong] = 0;
	A.DongTrong = A.DongTrong + 1;
	A.Previous = A.LuuViTri;
	return A;
}
void PrintPath(vector<STATE> &Path, char *output, int LuuViTri)
{
	FILE *f;
	errno_t err;
	err = fopen_s(&f, output, "w");
	vector<STATE> Next;
	int i;
	if (LuuViTri == 0)
		fprintf(f, "NULL");
	else
	{
		Next.push_back(Path[LuuViTri]);
		do
		{
			i = Next[Next.size() - 1].Previous;
			if (i > -1)
			{
				Next.push_back(Path[i]);
			}
		} while (i > -1);

		cout << Next.size() << endl << endl;
		fprintf_s(f, "%d\n\n", Next.size());
		for (int k = Next.size() - 1; k >= 0; k--)
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					cout << Next[k].Taci[i][j] << " ";
					fprintf_s(f, "%d ", Next[k].Taci[i][j]);
				}
				cout << endl;
				fprintf_s(f, "\n");
			}
			cout << endl << endl;
			fprintf_s(f, "\n");
		}
	}
	fclose(f);
}
int Heuristic(STATE A, STATE end)
{
	int h = 0;
	int xA = 0, yA = 0, xend = 0, yend = 0;
	for (int k = 1; k < 16; k++)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				if (A.Taci[i][j] != end.Taci[i][j])
					h++;
				if (A.Taci[i][j] == k)
				{
					xA = i;
					yA = j;
				}
				if (end.Taci[i][j] == k)
				{
					xend = i;
					yend = j;
				}
			}
		h = h + pow(abs(xA - xend), 2) + pow(abs(yA - yend), 2);
		h = h - (0.15*h);
		h = h + calInversions(A);

	}
	return h;
}

bool operator > (const STATE &A, const STATE &B){
	return (A.h + A.g) < (B.h + B.g);
}
bool operator < (const STATE &A, const STATE &B){
	return (A.h + A.g) > (B.h + B.g);
}
int calInversions(STATE C)
{
	int dem = 0;
	int puzzleArray[16];
	int f = 0;
	for (int o = 0; o < 4; o++)
	{
		for (int p = 0; p < 4; p++)
		{
			puzzleArray[f] = C.Taci[o][p];
			f++;
		}
	}
	for (int k = 0; k < 16; k++)
	{
		for (int m = k; m < 16; m++)
		{
			if (puzzleArray[k] > puzzleArray[m] && puzzleArray[m] != 0)
				dem++;
		}
	}
	return dem;
}
bool checkState(STATE C)
{
	int inversions = 0;
	inversions = calInversions(C);
	if ((C.DongTrong) % 2 == 0 && inversions % 2 != 0)
		return true;
	if ((C.DongTrong) % 2 != 0 && inversions % 2 == 0)
		return true;
	return false;
}
int Asolver(STATE start, STATE end, vector<STATE> &Path)
{
	int i = 0, j = 0, x = 1;
	priority_queue<STATE, vector<STATE>> q;
	q.push(start);
	if (!(checkState(start)))
		return 0;
	Path.push_back(start);
	while (q.empty() != true)
	{
		STATE A = q.top();
		if (check(A, end) == 1)
			return A.LuuViTri;
		q.pop();
		if (A.CotTrong > 0)
		{
			STATE B = ChuyenTrai(A);
			if (A.Previous == -1 || check(Path[A.Previous], B) == 0)
			{
				B.LuuViTri = Path.size();
				B.g = B.g + 1;
				B.h = Heuristic(B, end);
				q.push(B);
				Path.push_back(B);
			}
		}
		if (A.DongTrong > 0)
		{
			STATE B = ChuyenTren(A);
			if (A.Previous == -1 || check(Path[A.Previous], B) == 0)
			{
				B.LuuViTri = Path.size();
				B.g = B.g + 1;
				B.h = Heuristic(B, end);
				q.push(B);
				Path.push_back(B);
			}

		}

		if (A.CotTrong < 3)
		{
			STATE B = ChuyenPhai(A);
			if (A.Previous == -1 || check(Path[A.Previous], B) == 0)
			{
				B.LuuViTri = Path.size();
				B.g = B.g + 1;
				B.h = Heuristic(B, end);
				q.push(B);
				Path.push_back(B);
			}
		}
		if (A.DongTrong < 3)
		{
			STATE B = ChuyenDuoi(A);

			if (A.Previous == -1 || check(Path[A.Previous], B) == 0)
			{
				B.LuuViTri = Path.size();
				B.g = B.g + 1;
				B.h = Heuristic(B, end);
				q.push(B);
				Path.push_back(B);
			}
		}
		printf("path size : %d\n", Path.size());
	}
	printf("done");
	return 0;
}