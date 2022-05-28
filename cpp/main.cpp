/**
 * 完善 Matrix 类，使得下面的代码可以正常运行:
 * class Matrix {
 * public:
 * TODO: Someting
 * private:
 *     int rows;
 *     int cols;
 *     int **matrix;
 * };
 *
 * 评分标准：
 * 1. 代码能够正常的运行就有 100 分，若代码不能正常运行，则按以下标准进行评分：
 * Matrix::Matrix(int rows, int cols):                              20 points
 * Matrix::~Matrix():                                               20 points
 * std::istream& operator>>(std::istream& in, Matrix& matrix)       20 points
 * std::ostream& operator<<(std::ostream& os, const Matrix& matrix) 20 points
 * Matrix Matrix::operator*(Matrix &other)                          20 points
 * 2. 在正常运行的基础上，若还有其他的功能，则按以下标准进行评分：
 * Matrix::Matrix(int rows, int cols, int** matrix)                 10 points
 * template <...> Matrix::Matrix(int (&matrix)[rows][cols])         10 points
 * Matrix::Matrix(const Matrix& other)                              10 points
 * Matrix::Matrix(Matrix&& other)                                   10 points
 * Matrix& Matrix::operator=(const Matrix& other)                   10 points
 * Matrix& Matrix::operator=(Matrix &&other)                        10 points
 * Matrix Matrix::operator+(const Matrix& other)                    10 points
 * Matrix Matrix::operator-(const Matrix& other)                    10 points
 * Matrix Matrix::operator/(const Matrix& other)                    10 points
 * bool Matrix::operator==(const Matrix& other)                     10 points
 */

#include<iostream>
class Matrix{
public:
    Matrix(int row,int col);
    Matrix(int row, int col, int** matrix) ;
    Matrix(const Matrix& m);
    Matrix(Matrix&& m);
    int GetRow()const;
    int GetCol()const;
    int * operator[](int row);
    Matrix& operator=(const Matrix& other);
    Matrix operator*(const Matrix& b)const;
    Matrix operator+(const Matrix& b)const;
    bool operator==(const Matrix& b)const;
    friend std::istream &operator >>(std::istream & in,Matrix & r);
    friend std::ostream &operator <<(std::ostream & out,const Matrix & r);
    ~Matrix();
private:
    int rows;
    int cols;
    int **matrix;
};
#include<iostream>
#include"matrix.h"
Matrix::Matrix(int row,int col){
    rows=row;
    cols=col;
    matrix=new int*[row];
    for(int i=0;i<row;i++){
            matrix[i] = new int[col];
        for(int j=0;j<col;j++){
            matrix[i][j] = 0;
        }
    }
}
Matrix::Matrix(int row, int col, int** p) {
    rows=row;
    cols=col;
    matrix = new int*[row];
    for(int i=0;i<rows;i++){
        matrix[i] = new int[cols];
        for(int j=0;j<cols;j++){
            matrix[i][j]=*((*p+i)+j);
        }
    }
}
Matrix::Matrix(const Matrix& m) {
	rows = m.GetRow();
	cols= m.GetCol();
	matrix = new int*[rows];
	for (int i = 0; i < rows; i++){
		matrix[i] = new int[cols];
	    for (int j = 0; j <  cols; j++){
		    matrix[i][j] = m.matrix[i][j];
	    }
    }
}
Matrix::Matrix(Matrix&& m){
    rows = m.GetRow();
	cols= m.GetCol();
	matrix = new int*[rows];
	for (int i = 0; i < rows; i++){
		matrix[i] = new int[cols];
	    for (int j = 0; j <  cols; j++){
		    matrix[i][j] = m.matrix[i][j];
	    }
    }
    m.~Matrix();
}
Matrix& Matrix::operator=(const Matrix& other){
    if (!(rows == other.rows&&cols == other.cols)) {	//尺寸不同重新构造
		this->~Matrix();								//释放掉先前的矩阵
		rows = other.rows;
		cols = other.cols;
        matrix = new int *[rows];
		for(int i=0;i < rows;i++){
            matrix[i] =new int[cols];
        }
	}
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			matrix[i][j] = other.matrix[i][j];
		}
	}
	return *this;
}
// Matrix operator *(Matrix &a, Matrix &b){
//     Matrix c(a.GetRow(),b.GetCol());
//     for (int i = 0; i < a.GetRow(); i++){
// 		for(int j=0;j<b.GetCol(); j++){
//             int s = 0;
// 			for (int k = 0; k < a.GetCol(); k++)
// 			{
// 				s += a[i][k] * b[k][j];
// 			}
//             c[i][j]=s;
//         }
// 	}
// 	return c;
// }
Matrix Matrix::operator * (const Matrix& b)const {
     Matrix c(rows,b.cols);
	for (int i = 0; i < rows; i++){
		for(int j=0;j<b.cols; j++){
            int s = 0;
			for (int k = 0; k < cols; k++){
				s += matrix[i][k] * b.matrix[k][j];
			}
            c.matrix[i][j]=s;
        }
	}
	return c;
}
Matrix Matrix::operator+ (const Matrix& b)const{
    Matrix c(rows,cols);
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            c.matrix[i][j]=matrix[i][j]+b.matrix[i][j];
        }
    }
    return c;
}
bool Matrix::operator==(const Matrix& b)const{
    int flag = 1;
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            if(matrix[i][j]!=b.matrix[i][j]){
                flag=0;
            }
        }
    }
    if(flag==1){
        return true;
    }
    else{
        return false;
    }
}
std::istream &operator >>(std::istream &in,Matrix &r){
    for(int i=0;i<r.rows;i++){
        for(int j=0;j<r.cols;j++){
            in >> r.matrix[i][j];
        }
    }
    return in;
}
std::ostream &operator <<(std::ostream &out,const Matrix &r){
    for(int i=0;i<r.rows;i++){
        for(int j=0;j<r.cols;j++){
            out << r.matrix[i][j];
        }
        out << '\n';
    }
    return out;
}
int Matrix::GetRow() const{ return rows; }
int Matrix:: GetCol() const{ return cols; }
Matrix::~Matrix(){
    for(int i=0;i<rows;i++){
        delete[] matrix[i];
    }
    delete[] matrix;
}
int* Matrix::operator[](int index){
	return *(matrix + index);
}
int main() {
    int rows, cols;

    std::cin >> rows >> cols;
    Matrix A(rows, cols);
    std::cin >> A;

    std::cin >> rows >> cols;
    Matrix B(rows, cols);
    std::cin >> B;

    std::cout << A * B;

    return 0;
}
