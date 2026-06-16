class StringMap
{
public:
    void insert(const std::string& key, int value);

    bool find(const std::string& key, int& value) const;

    bool erase(const std::string& key);

    size_t size() const;
};

Do NOT use
    std::unordered_map
    std::map
    Any existing hash table implementation
Implement your own:
    Hash function
    Bucket management
    Collision handling
Complexity targets:
    Operation	Average
    insert	O(1)
    find	O(1)
    erase	O(1)

