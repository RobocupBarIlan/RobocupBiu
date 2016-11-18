#ifndef SHAREDMEMORY_H_
#define SHAREDMEMORY_H_


#include <semaphore.h>

template<typename T>
class SharedMemory {
public:
	SharedMemory(T object){
		sem_init(&m_semaphore, 0, 1);
		m_shraedObject = object;
	}
	SharedMemory(){
		sem_init(&m_semaphore, 0, 1);
	}
	~SharedMemory(){
		sem_destroy(&m_semaphore);
	}

	T SafeRead(){
		if (sem_wait(&m_semaphore) != 0){
			cout << "Couldn't sem_wait\n";
		}
		T objectToReturn = m_shraedObject;
		if (sem_post(&m_semaphore) != 0){
			cout << "Couldn't sem_post\n";
		}
		return objectToReturn;
	}
	void SafeWrite(T objectToWrite){
		if (sem_wait(&m_semaphore) != 0){
			cout << "Couldn't sem_wait\n";
		}
		// Free the memory of the old class, then store the new pointer.
		// delete m_shraedObject;
		m_shraedObject = objectToWrite;
		if (sem_post(&m_semaphore) != 0){
			cout << "Couldn't sem_post\n";
		}
	}

private:
	sem_t m_semaphore;
	T m_shraedObject;
};

#endif /* SHAREDMEMORY_H_ */
