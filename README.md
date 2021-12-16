# Bidirectional List
#### __What does it contain?__:
- This project contains a **bidirectional list** - a fast data structure that is working on RAM memory.
- You can view all class methods at the top of .hpp file - each one has short explanation
- Many useful public methods - except of typical vector methods you also have there other useful ones, such as:
    - importing arrays to list
    - counting elements in list
    - finding position of first found element
    - priting all data contained by list 

#### __How to sort or print custom structures?__ :

It's not that had, however - it needs some user's knowledge. Certainly, he has to write his own comparison function or lambda expression and add in-structure method for operator <<. It is also important to note that it uses quicksort algorithm.

Example:
```cpp
struct point {
    int x;
    int y;
    friend std::ostream& operator<< (std::ostream& output, point& p); // declaration of operator << behavior - otherwise printList() method will crash as it can't print this structure
};

std::ostream& operator<< (std::ostream& output, point& p) { 
    output << "{ " << p.x << ", " << p.y << " }";
    return output;
} // definition of operator << behavior - user is supossed to state how does he want to print structure and it's data

int main() {
    List<point> l1 = { { 5, 3 }, { 8, 1 }, { 3, 0 } };
    l1.sort([](point x, point y) { return x.x > y.x; }); // lambda expression of sorting condition - here it sorts ascending by x variable of point structure
    l1.printList();    
}
```
The output is:
``` [{ 3, 0 }, { 5, 3 }, { 8, 1 }] ```

> ### Thank you for reading and using my stuff! If you have any complains or would like to suggest some other functionalities then feel free to ask me on Discord - cub8#9571.