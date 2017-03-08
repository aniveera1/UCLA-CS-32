void listAll(const MenuItem* m, string path) // two-parameter overload
{
    // Check against invalid access
    if (m == nullptr)
        return;
    
    // Get current path to current item
    string currentPath;
    if (path == "")
        currentPath = m->name();
    else
        currentPath = path + "/" + m->name();
    
    // Check whether MenuItem is a
    // PlainMenuItemm
    if (m->menuItems() == nullptr)
    {
        cout << currentPath << endl;
        return;
    }
    
    // Check whether CompoundMenuItem
    // has no child MenuItems
    if (m->menuItems()->size() == 0)
    {
        cout << currentPath << endl;
        return;
    }
    
    // Otherwise print currentPath
    if (currentPath != "")
        cout << currentPath << endl;
    
    // Iterate through all child MenuItems
    for (int i = 0; i < m->menuItems()->size(); i++)
    {
        const vector<MenuItem*>* menuItems = m->menuItems();
        listAll((*menuItems)[i], currentPath);
    }
}
