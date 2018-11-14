#include "mpi.h"
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <ctime>
#include <cmath>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
	if (argc < 2)
		return 0;

	int size, rank;
	int dims[2];
	int coords[2];
	int period[2] = { 1,1 };
	int sending = atoi(argv[1]);
	int recieving = atoi(argv[2]);
	MPI_Comm comm;
	MPI_Status *status = new MPI_Status();
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (size > 1 && sending >= 0 && sending < size && recieving >= 0 && recieving < size)
	{
		dims[0] = 1;
		dims[1] = size;
		MPI_Cart_create(MPI_COMM_WORLD, 2, dims, period, 1, &comm);

		int direction;
		if (sending < recieving) direction = 1;
		if (sending > recieving) direction = -1;
		if (sending == recieving) direction = 0;


		MPI_Cart_coords(comm, rank, 2, coords);
		char *str = "";
		int length;

		if (rank == sending)
		{
			string message = " sdzgfvksdgdâgdfikhdfhffdbdfbsknvdknv";
			length = message.length();
			str = new char[length];
			for (int i = 0; i < length; i++)
			{
				str[i] = message[i];
			}

			if (sending == recieving)
			{
				cout << "rank = " << rank << " I sent!" << endl;
				cout << "rank = " << rank << " Yes ! I received" << endl;
				for (int i = 0; i < length; i++)
				{
					cout << str[i];
				}
			}
			else
			{
				int tmp;
				coords[1] = coords[1] + direction;
				MPI_Cart_rank(comm, coords, &tmp);
				MPI_Comm_rank(comm, &rank);
				MPI_Send(&length, 1, MPI_INT, tmp, 1, comm);
				MPI_Send(str, length, MPI_CHAR, tmp, 1, comm);
				cout << "rank = " << rank << " I sent" << endl;
			}
		}
		else if (rank == recieving)
		{
			MPI_Recv(&length, 1, MPI_INT, MPI_ANY_SOURCE, 1, comm, status);
			str = new char[length];
			MPI_Recv(str, length, MPI_CHAR, MPI_ANY_SOURCE, 1, comm, status);
			cout << "rank = " << rank << "  Yes ! I received" << endl;

			for (int i = 0; i < length; i++)
			{
				cout << str[i];
			}
		}
		else if ((rank > recieving && rank < sending) || (rank < recieving && rank > sending))
		{
			MPI_Recv(&length, 1, MPI_INT, MPI_ANY_SOURCE, 1, comm, status);
			str = new char[length];
			MPI_Recv(str, length, MPI_CHAR, MPI_ANY_SOURCE, 1, comm, status);
			int tmp;
			coords[1] = coords[1] + direction;
			MPI_Cart_rank(comm, coords, &tmp);
			MPI_Send(&length, 1, MPI_INT, tmp, 1, comm);
			MPI_Send(str, length, MPI_CHAR, tmp, 1, comm);
		}
	}
	MPI_Finalize();
	return 0;
}
