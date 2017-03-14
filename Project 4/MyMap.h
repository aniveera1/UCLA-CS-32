#ifndef MYMAP_INCLUDED
#define MYMAP_INCLUDED

// MyMap.h

////////////////////////////////////////////////
// Declaration
////////////////////////////////////////////////

template <typename KeyType, typename ValueType>
class MyMap
{
public:
	// Constructor
	MyMap(); 

	// Destructor
	~MyMap(); 

	// Deletes all associations in the tree
	void clear();

	// Returns the number of associations
	int size() const; 

	// Creates a new association or updates
    // an existing one
	void associate(const KeyType& key, const ValueType& value);

	// Returns a pointer to the value of a
    // given key, or nullptr if the key
    // does not exist
	const ValueType* find(const KeyType& key) const;

    // Return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
    }
    
    // Prevents copying and assignment
    MyMap(const MyMap&) = delete;
    MyMap& operator=(const MyMap&) = delete;
private:
    // Node struct
	struct BSTNode
	{
        // Constructor
        BSTNode(KeyType key, ValueType value)
        : m_key(key), m_value(value), m_left(nullptr), m_right(nullptr)
        {
        }
        
		KeyType		m_key;
        ValueType   m_value;
        BSTNode*    m_left;
        BSTNode*    m_right;
    };
    
    // Root pointer
    BSTNode*        m_root;
    
    // Size tracker
    int             m_size;
    
    // Helper Function for clear()
    void deleteMe(BSTNode* me);
    
    // Helper Function for associate()
    void addNode(const KeyType& key, const ValueType& value);
};

////////////////////////////////////////////////
// Public Functions
////////////////////////////////////////////////

// Constructor
template <typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::MyMap()
: m_root(nullptr), m_size(0)
{
}

// Destructor
template <typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::~MyMap()
{
    this->clear();
}

// Deletes all associations in the tree
template <typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::clear()
{
    this->deleteMe(m_root);
    m_size = 0;
    m_root = nullptr;
}

// Returns the number of associations
template <typename KeyType, typename ValueType>
int MyMap<KeyType, ValueType>::size() const
{
    return m_size;
}

// Creates a new association or updates
// an existing one
template <typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
    // Initial setup
    bool keyExists = false;
    BSTNode* current = m_root;
    
    // Iterate through tree and see if key exists
    while (current != nullptr && ! keyExists)
    {
        if (current->m_key == key)
            keyExists = true;
        else if (current->m_key > key)
            current = current->m_left;
        else
            current = current->m_right;
    }
    
    // If association with key already exists,
    // replace association with new value
    if (keyExists)
    {
        current->m_value = value;
        return;
    }
    
    // If no association exists, allocate a new
    // node with the given key and value and
    // update size
    this->addNode(key, value);
    m_size++;
}

// Returns a pointer to the value of a
// given key, or nullptr if the key
// does not exist
template <typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType, ValueType>::find(const KeyType& key) const
{
    // Iterate through tree, and return
    // association if found
    BSTNode* current = m_root;
    while (current != nullptr)
    {
        if (current->m_key == key)
            return &(current->m_value);
        else if (current->m_key > key)
            current = current->m_left;
        else
            current = current->m_right;
    }
    
    // No association found
    return nullptr;
}

////////////////////////////////////////////////
// Helper Functions
////////////////////////////////////////////////

// Helper function for clear()
template <typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::deleteMe(BSTNode* me)
{
    // If node does not exist,
    // simply return
    if (me == nullptr)
        return;
    
    // Delete all nodes on left
    deleteMe(me->m_left);
    
    // Delete all nodes on right
    deleteMe(me->m_right);
    
    // Delete current node
    delete me;
}

// Helper Function for associate()
template <typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::addNode(const KeyType& key, const ValueType& value)
{
    // If tree is empty, simply allocate
    // a new node
    if (m_root == nullptr)
    {
        m_root = new BSTNode(key, value);
        return;
    }
    
    // Iterate through tree and find an
    // empty spot to allocate the new node
    BSTNode* current = m_root;
    while (true)
    {
        // Parent node is greater therefore
        // we go left
        if (current->m_key > key)
        {
            // If spot is empty, then add node
            if (current->m_left == nullptr)
            {
                current->m_left = new BSTNode(key, value);
                return;
            }
            current = current->m_left;
        }
        // Otherwise we go right
        else
        {
            // If spot is empty, then add node
            if (current->m_right == nullptr)
            {
                current->m_right = new BSTNode(key, value);
                return;
            }
            current = current->m_right;
        }
    }
}

#endif // MYMAP_INCLUDED
