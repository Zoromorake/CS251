#pragma once
#include "cstring"
#include "sstream"
#include "string"
//#include "iostream"

namespace cs251
{
	template<typename T>
	class cyclic_double_queue
	{
	public:
		/**
		 * \brief The constructor with settings.
		 * \param initialCapacity Preset initial capacity of the array.
		 * \param increaseFactor Preset increase factor for expanding the array.
		 * \param decreaseFactor Preset decrease factor for shrinking the array.
		 */
		cyclic_double_queue(size_t initialCapacity = 7, size_t increaseFactor = 2, size_t decreaseFactor = 2);
		/**
		 * \brief The default destructor. You should make sure no memory leaks happens after the queue is destroyed.
		 */
		~cyclic_double_queue();
		/**
		 * \brief Push the item at the front of the queue.
		 * \param item The item to be added.
		 */
		void enqueue_front(const T& item);
		/**
		 * \brief Push the item at the back of the queue.
		 * \param item The item to be added.
		 */
		void enqueue_back(const T& item);
		/**
		 * \brief Returns the occupancy of the queue.
		 * \return Whether the queue is empty.
		 */
		bool empty() const;
		/**
		 * \brief Dequeue the element at the front of the queue.
		 * \return The item dequeued.
		 */
		T dequeue_front();
		/**
		 * \brief Dequeue the element at the back of the queue.
		 * \return The item dequeued.
		 */
		T dequeue_back();
		/**
		 * \brief Remove the item at the front of the queue, returns nothing.
		 */
		void pop_front();
		/**
		 * \brief Remove the item at the back of the queue, returns nothing.
		 */
		void pop_back();
		/**
		 * \brief Provide the access to the item at the front of the queue.
		 * \return The modifiable reference to the item.
		 */
		T& front();
		/**
		 * \brief Provide the access to the item at the back of the queue.
		 * \return The modifiable reference to the item.
		 */
		T& back();
		/**
		 * \brief Empty the queue, and resize the queue to initial capacity.
		 * \n !Note! You should set the size and the front index of the queue to 0 when this function is called!
		 */
		void clear();
		/**
		 * \brief Print the queue's current status according to the handout.
		 * \return The string represents the array. For format please refer to the handout.
		 */
		std::string print_status() const;
		/**
		 * \brief Returns the number of items currently in queue.
		 * \return The size of the queue.
		 */
		size_t get_size() const;
		/**
		 * \brief Returns the total capacity of the array.
		 * \return The capacity of the array.
		 */
		size_t get_capacity() const;
	private:
		//TODO: You may add private members/methods here.
		/**
		 * Back Index of Queue within Array
		 */
		size_t m_backIndex = 0; //My Make
		/**
		 * Resize Void Funct
		*/
		void resize(size_t newCapacity); //My Make
		/**
		 * Item array, the underlying data structure for the queue
		 */
		T* m_data = nullptr;
		/**
		 * The front index of the queue within the array
		 */
		size_t m_frontIndex = 0;
		/**
		 * The length of the array, including the empty slots.
		 */
		size_t m_capacity = 0;
		/**
		 * Keeps track of the size of the queue
		 */
		size_t m_size = 0;
		/**
		 * Factor by which the queue length must be increased 
		 */
		size_t m_increaseFactor = 0;
		/**
		 * Factor by which the queue length must be decreased
		 */
		size_t m_decreaseFactor = 0;
		/**
		 * The initial capacity of the queue, predefined as 7
		 */
		size_t m_initialCapacity = 0;
	};

	template <typename T>
	cyclic_double_queue<T>::cyclic_double_queue(const size_t initialCapacity, const size_t increaseFactor, size_t decreaseFactor)
		:m_initialCapacity(initialCapacity), m_increaseFactor(increaseFactor), m_decreaseFactor(decreaseFactor)
	{
		//TODO: Remove following line and add your implementation here.
		m_capacity = m_initialCapacity; //sets mcap to chosen initial cap
		m_data = new T[m_capacity];		//makes array with capacity above
	}

	template <typename T>
	cyclic_double_queue<T>::~cyclic_double_queue()
	{
		//TODO: Add your implementation here.
		delete[] m_data; //deletes data from the queue
	}

	template <typename T>
	void cyclic_double_queue<T>::enqueue_front(const T& item)
	{
		//TODO: Remove following line and add your implementation here.
		//First move/get index F then Insert
		if (m_size == m_capacity) {
			resize(m_capacity * m_increaseFactor); //Increases array size if needed
		}
		
		
		if (m_frontIndex == 0) { //m_size == 0 &&
			m_frontIndex = m_capacity - 1; //[b][-][f]
		} else {
			m_frontIndex = (m_frontIndex - 1 + m_capacity) % m_capacity;
		}
		//std::cout << m_frontIndex;
		m_data[m_frontIndex] = item; //Puts item in the front queue
		++m_size; //increases the queue size count
	}

	template <typename T>
	void cyclic_double_queue<T>::enqueue_back(const T& item)
	{
		//TODO: Remove following line and add your implementation here.
		//Puts it in current back, moves back up
		if (m_size == m_capacity) {
			resize(m_capacity * m_increaseFactor); //Increases array size if needed
		}

		m_backIndex = (m_frontIndex + m_size) % m_capacity; //locates back index
		m_data[m_backIndex] = item; //Puts item in the back of queue
		
		//std::cout << m_backIndex;
		++m_size; //increases the queue size count
		m_backIndex++; //Moves back index up 1
	}

	template <typename T>
	T cyclic_double_queue<T>::dequeue_front() //works correct not
	{
		//TODO: Remove following line and add your implementation here.
		if (m_size == 0) {
			throw std::out_of_range("cyclic_double_queue is empty!"); //throws exception
		}
		T frontItem = m_data[m_frontIndex]; //Stores front item in local var
		m_data[m_frontIndex] = T(); //Clears front item from array
		m_frontIndex = (m_frontIndex+1) % m_capacity; //Locates new front index
		--m_size; //Decreases Queue Size
		
		if (m_capacity > m_size * 4) {
            if (m_capacity != m_initialCapacity) {
				resize(m_capacity / m_decreaseFactor); //Decreases array size, but keeps it above initcap
			}
		}
		if (m_size == 1 && (m_backIndex-m_frontIndex == 1)) {
			m_backIndex = m_frontIndex;
		}
		return frontItem; //returns the front item		
	}

	template <typename T>
	T cyclic_double_queue<T>::dequeue_back() //Wroks Correct
	{
		//TODO: Remove following line and add your implementation here.
		if (m_size == 0) {
			throw std::out_of_range("cyclic_double_queue is empty!"); //throws exception
		}
		size_t lbackIndex = ((m_frontIndex+m_size-1) % m_capacity); //finds current backindex - 1
		//add some condition for test 7
		
		T backItem = m_data[lbackIndex]; //Stores back item
		m_data [lbackIndex] = T(); //deletes item from array
		m_backIndex = (m_frontIndex+m_size-1) % m_capacity; //sets back index to new locat
		--m_size; //Decreases Queue Size
		
		if (m_capacity > m_size * 4) {
            if (m_capacity != m_initialCapacity) {
				resize(m_capacity / m_decreaseFactor); //Decreases array size, but keeps it above initcap
			}
		}
		if (m_size == 1 && (m_backIndex-m_frontIndex == 1) && m_frontIndex == 0) {
			m_backIndex = m_frontIndex;
		}
		return backItem; //returns the back item
	}

	template <typename T>
	void cyclic_double_queue<T>::pop_front()
	{
		//TODO: Remove following line and add your implementation here.
		dequeue_front();
		
	}

	template <typename T>
	void cyclic_double_queue<T>::pop_back()
	{
		//TODO: Remove following line and add your implementation here.
		dequeue_back();
	}

	template <typename T>
	bool cyclic_double_queue<T>::empty() const
	{
		//TODO: Remove following line and add your implementation here.
		return m_size==0; //Sees if queue size is 0
	}

	template <typename T>
	T& cyclic_double_queue<T>::front()
	{
		//TODO: Remove following line and add your implementation here.

		if (m_size == 0) {
            throw std::out_of_range("cyclic_double_queue is empty!");
        }
		
		//std::cout << "Front Index Location: " << m_frontIndex << "\n";
        return m_data[m_frontIndex]; //returns frontmost item val
	}

	template <typename T>
	T& cyclic_double_queue<T>::back()
	{
		//Correct?
		if (m_size == 0) {
            throw std::out_of_range("cyclic_double_queue is empty!");
        }
		size_t backIndex = (m_frontIndex+m_size-1) % m_capacity;
		if (m_size == m_capacity) {
			backIndex = m_frontIndex;
		}
		//std::cout << "Back Index Location: " << m_backIndex << "\n";
        return m_data[backIndex]; //returns back index item val
		
	}

	template <typename T>
	void cyclic_double_queue<T>::clear()
	{
		//Correct
		delete[] m_data;
		m_capacity = m_initialCapacity;
		m_size = 0; //clears everything as per description
		m_data = new T[m_capacity];
		m_frontIndex = 0;
		m_backIndex = 0;
		
	}

	template <typename T>
	std::string cyclic_double_queue<T>::print_status() const
	{
		//Catch the initial - when size > 0
		//Needs work
		std::string ocpd;
		for (size_t i=0; i<m_capacity; ++i) {
			ocpd += "["; //Prints open bracket
			if (m_size == 0) { //at size 0 whole array is unoccupied
				ocpd += "-";
			} else if (m_frontIndex < m_backIndex) { //right here is the problem
				if (i < m_backIndex && i >= m_frontIndex) {
					ocpd += "+"; //Occupied slot
				} else {
					ocpd += "-"; //Not Occupied Slot
				}	
			} else if (m_frontIndex > m_backIndex) {
				if (i < m_backIndex || i >= m_frontIndex) {
					ocpd += "+"; //Occupied slot
				} else {
					ocpd += "-"; //Not Occupied Slot
				}
			} else if (m_frontIndex == m_backIndex) {
				if (i < m_backIndex || i >= m_frontIndex) {
					ocpd += "+"; //Occupied slot
				} else {
					ocpd += "-"; //Not Occupied Slot
				}
			}
			ocpd += "]"; //prints closed bracket	
		}
		return ocpd; //returns the occupied slots string
	}

	template <typename T>
	size_t cyclic_double_queue<T>::get_size() const
	{
		//Correct
		return m_size;
	}

	template <typename T>
	size_t cyclic_double_queue<T>::get_capacity() const
	{
		//Correct
		return m_capacity;
	}

	template <typename T>
    void cyclic_double_queue<T>::resize(size_t newCapacity) //resizes and creates new array
    { //Corrected
		T* new_data = new T[newCapacity]; //creates new array
        for (size_t i = 0; i < m_size; ++i) { //copies over old data
			
			new_data[i] = m_data[(m_frontIndex + i) % m_capacity];
        }
        delete[] m_data; //deletes old array
        m_data = new_data; //sets new data to m data var
        m_capacity = newCapacity; //sets new cap to m cap var
        m_frontIndex = 0; //sets front index to 0
		m_backIndex = m_size;
		
	} 
}
