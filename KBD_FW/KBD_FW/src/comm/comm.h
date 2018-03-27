/*
 * comm.h
 *
 * Created: 3/18/2018 9:03:49 PM
 *  Author: qis4q
 */ 


#ifndef COMM_H_
#define COMM_H_

#define BYTES_PER_PIXEL				1
#define FILE_READ_BUFFER_SIZE		12*(2+1+1+1+1+BYTES_PER_PIXEL*50*50)
#define FILE_SECTION_IDENTIFIER		0xDEAD

void comm_init(void);

void comm_process(void);



#endif /* COMM_H_ */