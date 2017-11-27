//Saúl Enrique Labra Cruz A01020725
//Manuel Guadarrama Vilalarroel A01020829
//Alberto Pascal Garza A01023607


#include <mpi.h>
#include <stdio.h>
#include <omp.h>
#include <vector>
#include <cmath>
#include <iostream>
using namespace std;
class matrix
{
public:
  vector<vector<double>> matrixData; // M: Filas, N:Columnas
  int M;
  int N;

  matrix(){};

  matrix(int M, int N)
  {
    this->M = M;
    this->N = N;
    matrixData.resize(M);

    for(int i=0; i<M; i++)
    {
      matrixData[i].resize(N);
    }

    cout << "se definio matriz de " << M << " x " << N << endl;
  }

  void populate(unsigned int tiempoSemilla, int rango)
  {
    int i,j;
    unsigned int seed = time(NULL);

    #pragma omp parallel private(i, j)
    {
      srand(pow(time(NULL), omp_get_thread_num()));

      #pragma omp for
      for(i=0; i<M; i++)
      {
        for(j=0; j<N; j++)
        {
          matrixData[i][j] = rand_r(&seed)%rango;
        }
      }
    }
    cout << "Se pobló la matriz con números aleatorios" << endl;
  }

  void printMatrix()
  {
    for(int i=0; i<M; i++)
    {
      for(int j=0; j<N; j++)
      {
        cout << matrixData[i][j] << "\t";
      }
      cout << endl;
    }
  }

  matrix mult(matrix B)
  {
    if(N == B.M)
    {
      int i, j, k;
      matrix C(M, B.N);

      #pragma omp parallel shared(B, C)
      {
        #pragma omp for private(i, j, k)
        for(i=0; i<M; i++)
        {
          for(j=0; j<B.N; j++)
          {
            C.matrixData[i][j] = 0;
            for(k = 0; k<N; k++)
            {
              C.matrixData[i][j] += matrixData[i][k] * B.matrixData[k][j];
            }
          }
        }
      }
      return C;
    }else{
      cout << "error: A.N debe de ser igual a B.M" << endl;
      return *this;
    }
  }
};
class Vector
{
public:
	int n; //para saber cuántos componentes tiene el vector
	vector<double> v;
	double magnitud=0;
	//char nom;
	Vector(int nums)
	{
        srand(time(NULL));
		this->n=nums;
		LlenarVector(time(NULL), 100);
		Consiguemagnitud();
        v.resize(n);
        cout<<"Vector de tamanio: "<<n<<endl;

	}
	Vector(){}
    void LlenarVector(unsigned int tiempoSemilla, int rango)
      {
        int i,j;
        unsigned int seed = time(NULL);

        #pragma omp parallel private(i, j)
        {
         
            
          #pragma omp for
          for(i=0; i<n; i++)
          {
  //           srand((time(NULL)));
              v.push_back(rand()%rango);
          }
        }
        cout << "Se lleno el vector" << endl;
      }
	
	void Consiguemagnitud()
	{
		cout<<"entré";
        magnitud=0;
		for(int x=0; x<n; x++)
		{
			magnitud += pow(v[x],2);
			//cout<<"la magnitud de"<<nom<<" Magnitud lleva" <<magnitud<<endl;
		}

		magnitud = sqrt(magnitud);

		//return magnitud;
	}
	double CalculaAngulo(Vector b)
	{
		Vector a;
		//a.nom='c';
		int llena=0;
		for(llena=0; v[llena]!=NULL && b.v[llena]!=NULL;llena++)
		{
			a.v.push_back(v[llena]*b.v[llena]);
		}
		a.n=llena-1;
		a.Consiguemagnitud();
        cout<<"pude con la magnitud" <<endl;
		double seno= 0;
		seno = (a.magnitud / (magnitud * b.magnitud));
        cout<<"pude con el seno. mi seno es de "<< seno<< "con a de "<<a.magnitud<< " magnitud de "<<magnitud<< " y magnitud de b de " <<b.magnitud<<endl;
		double angulo = asin(seno);
        angulo= angulo * 360 / (2*3.141592); //convierte de radianes a grados. 
        cout<<"Mi angulo es de "<<angulo<<endl;
		
		return angulo;
		
	}
	double productoPunto(Vector b)
	{
		double angulo = CalculaAngulo(b);
		double producto= 0;
        angulo= angulo*2*3.141592/360;

		producto = magnitud * b.magnitud * cos(angulo);
	}
    void printVector()
      {
          for(int i=0; i<n; i++)
          {
            cout << v[i] << endl;
          }
          cout << endl;
      }

      Vector suma(Vector B)
      {
          if (n == B.n) {
              int i, j;
               Vector C(n);
              #pragma omp parallel shared(B, C)
              {
                #pragma omp for private(i, j)
                for(i=0; i<n; i++)
                {
                    C.v[i] += v[i] + B.v[i];
                }
              }
              return C;
          }else{
              cout<<"Debe de ser igual el tamanio de los vectores"<<endl;
              return *this;
          }

      }
};

int main()
{
	MPI_Init(NULL, NULL);
	//vector<int> a;
	//Vector Av(3);
	//A.nom='A';
//	cout<<A.magnitud;
	//Vector Bv(5);
	//B.nom='B';
//	cout<<endl<<B.magnitud;
	//double prueba=Av.CalculaAngulo(Bv);
	//cout<<endl<<prueba;
	//cout<<"Mi producto punto es de " <<Av.productoPunto(Bv)<<endl;
	// Get the name of the processor
    	char processor_name[MPI_MAX_PROCESSOR_NAME];
    	int name_len;
    	MPI_Get_processor_name(processor_name, &name_len);
cout<<"lo compila: "<<processor_name<<endl;

     matrix A(2, 3);
      matrix B(3, 4);

      A.populate(time(NULL), 10);
      A.printMatrix();

      B.populate(time(NULL), 10);
      B.printMatrix();

      matrix resultado = A.mult(B);
      resultado.printMatrix();
	MPI_Finalize();
      return 0;
}

