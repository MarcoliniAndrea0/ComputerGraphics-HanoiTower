#pragma one

#include <string>

class Object
{
public:
    Object(const std::string& name = "", const std::string& type = "");
    virtual ~Object() = default;

    virtual void render() = 0;

    int getID() const;
    const std::string& getName() const;
    const std::string& getType() const;

    void setName(const std::string& newName);
    void setType(const std::string& newType);

protected:
    int id;
    std::string name;
    std::string type;

private:
    static int id_counter;
};
