class Landmark
{
  public:
    Landmark(string nm);
    virtual ~Landmark();
    string name() const;
    virtual string icon() const = 0;
    virtual string color() const;
    
  private:
    string m_name;
};

class Hotel : public Landmark
{
public:
    Hotel(string name);
    virtual ~Hotel();
    virtual string icon() const;
};

class Restaurant : public Landmark
{
public:
    Restaurant(string name, int capacity);
    virtual ~Restaurant();
    virtual string icon() const;
    
private:
    int m_capacity;
    string m_icon;
};

class Hospital : public Landmark
{
public:
    Hospital(string name);
    virtual ~Hospital();
    virtual string color() const;
    virtual string icon() const;
};

Landmark::Landmark(string nm)
  :m_name(nm)
{}

Landmark::~Landmark()
{}

string Landmark::name() const
{
    return m_name;
}

string Landmark::color() const
{
    return "yellow";
}

Hotel::Hotel(string name)
  :Landmark(name)
{}

Hotel::~Hotel()
{
    cout << "Destroying the hotel " << name() << "." << endl;
}

string Hotel::icon() const
{
    return "bed";
}

Restaurant::Restaurant(string name, int capacity)
  :Landmark(name), m_capacity(capacity)
{
    if (capacity > 0 && capacity < 40)
        m_icon = "small knife/fork";
    else
        m_icon = "large knife/fork";
}

Restaurant::~Restaurant()
{
    cout << "Destroying the restaurant " << name() << "." << endl;
}

string Restaurant::icon() const
{
    return m_icon;
}

Hospital::Hospital(string name)
  :Landmark(name)
{}

Hospital::~Hospital()
{
    cout << "Destroying the hospital " << name() << "." << endl;
}

string Hospital::color() const
{
    return "blue";
}

string Hospital::icon() const
{
    return "H";
}