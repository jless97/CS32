#include "BinaryFile.h"
#include "DiskMultiMap.h"
#include "MultiMapTuple.h"
#include "IntelWeb.h"
#include "InteractionTuple.h"

#include <iostream>
using namespace std;

int main()
{
    DiskMultiMap x;
    x.createNew("myhashtable.dat", 100); // empty, with 100 buckets
    
    x.insert("hmm.exe", "pfft.exe", "m52902");
    x.insert("hmm.exe", "pfft.exe", "m52902");
    x.insert("hmm.exe", "pfft.exe", "m10001");
    x.insert("blah.exe", "bletch.exe", "m0003");
    
    DiskMultiMap::Iterator it = x.search("goober.exe");
    if (! it.isValid())
        cout << "I couldn't find goober.exe" << endl;
    cout << "====" << endl;
    
    it = x.search("hmm.exe");
    if (it.isValid())
    {
        cout << "I found at least 1 item with a key of hmm.exe" << endl;
        cout << "====" << endl;
        do
        {
            MultiMapTuple m = *it; //get the association
            cout << "The key is: " << m.key << endl;
            cout << "The value is: " << m.value << endl;
            cout << "The context is: " << m.context << endl;
            cout << endl;
            
            ++it; //advance iterator to the next matching item
        }while (it.isValid());
    }
    else
        cout << "Fail" << endl;
    
    if (x.erase("hmm.exe", "pfft.exe", "m52902") == 2)
        cout << "Just erased 2 items from the table!" << endl;
    
    if (x.erase("hmm.exe", "pfft.exe", "m10001") > 0)
        cout << "Just erased at least 1 item from the table!" << endl;
    
    if (x.erase("blah.exe", "bletch.exe", "m0003") == 1)
        cout << "Just erased 1 item from the table!" << endl;
    
    if (x.erase("blah.exe", "bletch.exe", "66666") == 0)
        cout << "I didn't erase this item cause it wasn't there" << endl;
    
    it = x.search("hmm.exe");
    if (it.isValid())
    {
        cout << "I found at least 1 item with a key of hmm.exe" << endl;
        cout << "====" << endl;
        do
        {
            MultiMapTuple m = *it; //get the association
            cout << "The key is: " << m.key << endl;
            cout << "The value is: " << m.value << endl;
            cout << "The context is: " << m.context << endl;
            cout << endl;
            
            ++it; //advance iterator to the next matching item
        }while (it.isValid());
    }
    else
        cout << "Fail" << endl;
    
    cout << "====" << endl;
    
    /*IntelWeb y;
    bool success = y.createNew("mydata", 10000);
    if (! success)
        cout << "fail" << endl;
    if (y.ingest("January-telemetry.dat"))
        cout << "Loaded data from January." << endl;
    if (y.ingest("February-telemetry.data"))
        cout << "Loaded data from February." << endl;*/
    
    return 0;
}







