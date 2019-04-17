void listAll(const Class* c, string path)  // two-parameter overload
{
    if (c->subclasses().empty())
        cout << path << c->name() << endl;
    else
    {
        path += c->name();
        cout << path << endl;
        path += "=>";
        for (vector<Class*>::const_iterator it = c->subclasses().begin(); it != c->subclasses().end(); it++)
        {
            listAll((*it), path);
        }
    }
    
}