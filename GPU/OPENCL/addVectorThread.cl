kernel void addVectorThread(global int* A,global int* B, global int* output, int size)
{
    int id =  get_global_id(0);    
    if (id < size)
    {        
        output[id] = A[i] + B[i];  
    }
}