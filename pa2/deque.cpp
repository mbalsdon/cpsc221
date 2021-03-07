/**
 * @file deque.cpp
 * Implementation of the Deque class.
 *
 */

template <class T>
Deque<T>::Deque(){

    n1 = 0;
    n2 = 0;

}

/**
 * Adds the parameter object to the right of the Deque.
 *
 * @param newItem object to be added to the Deque.
 */
template <class T>
void Deque<T>::pushR(T newItem)
{
    data.push_back(newItem);
}

/**
 * Removes the object at the left of the Deque, and returns it to the
 * caller.
 *
 * See .h file for implementation notes. 
 *
 * @return The item that used to be at the left of the Deque.
 */
template <class T>
T Deque<T>::popL()
{
    /* Retrieve and remove leftmost element */
    T popped = data.at(n1);
    // data.at(n1) = 0; // *** Wasn't behaving with treasureMap
    /* If last element is popped,  */
    if (data.size() - n1 <= 1) {
        n1 = 0;
        data.pop_back();
        return popped;
    } else {
    n1++;
    }
    /* Resize deque if empty space >= filled space */
    if ((int) data.size() - n1 <= n1) {
        vector<T> new_guy;
        /* Copy elements of old array to new array */
        for (unsigned i = n1; i < data.size(); i++) {
            new_guy.push_back(data.at(i));
        }
        n1 = 0;
        data = new_guy;
    }
    return popped;
}
/**
 * Removes the object at the right of the Deque, and returns it to the
 * caller.
 *
 * @return The item that used to be at the right of the Deque.
 */
template <class T>
T Deque<T>::popR()
{
    /* Retrieve and remove rightmost element */
    T popped = data.back();
    data.pop_back();
    /* Resize deque if empty space >= filled space */
    if ((int) data.size() - n1 <= n1) {
        vector<T> new_guy;
        /* Copy elements of old array to new array */
        for (unsigned i = n1; i < data.size(); i++) {
            new_guy.push_back(data.at(i));
        }
        n1 = 0;
        data = new_guy;
    }
    return popped;
}

/**
 * Finds the object at the left of the Deque, and returns it to the
 * caller. Unlike popL(), this operation does not alter the deque.
 *
 * @return The item at the front of the deque.
 */
template <class T>
T Deque<T>::peekL()
{
    return data.at(n1);
}

/**
 * Finds the object at the right of the Deque, and returns it to the
 * caller. Unlike popR(), this operation does not alter the deque.
 *
 * @return the value of The item at the right of the deque.
 */
template <class T>
T Deque<T>::peekR()
{
    return data.back();
}
/**
 * Determines if the Deque is empty.
 *
 * @return bool which is true if the Deque is empty, false otherwise.
 */
template <class T>
bool Deque<T>::isEmpty() const
{
    return data.empty();
}

/**
 * Determines the size of the Deque (Test function)
 * 
 * @return size of the Deque.
 */
template <class T>
int Deque<T>::size()
{
    return data.size();
}
