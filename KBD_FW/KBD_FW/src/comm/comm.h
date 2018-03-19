/*
 * comm.h
 *
 * Created: 3/18/2018 9:03:49 PM
 *  Author: qis4q
 */ 


#ifndef COMM_H_
#define COMM_H_

#define FILE_READ_BUFFER_SIZE		512//12*(2+1+1+1+1+3*50*50)

void comm_init();

void comm_process();



#endif /* COMM_H_ */