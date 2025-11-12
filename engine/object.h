#include <string>

class Object
{
public:
    Object();
    virtual ~Object() = default;

    virtual void render() = 0;
    void *getType();
    void setType(void *type);
    int generateID();

private:
    void *type;
    int id;
    std::string name;
};