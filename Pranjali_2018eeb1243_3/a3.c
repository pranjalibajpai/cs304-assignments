#include<stdio.h>
#include<limits.h>
#include<stdbool.h>
int main()
{
    // Number of nodes
    int n=5;
    // Name of Nodes
    char name[5]={'A', 'B', 'C', 'D', 'E'};
    // Initial Time 
    int t=0;
    
    // A=>0 B=>1 C=>2 D=>3 E=>4
    //      A--------B----
    //      |             |
    //      |             -----C
    //      |             |
    //      E--------D----

    // Initial routing table
    int table[5][5] = {
                        {0, 1, INT_MAX, INT_MAX, 1}, 
                        {1, 0, 1, INT_MAX, INT_MAX}, 
                        {INT_MAX, 1, 0, 1, INT_MAX}, 
                        {INT_MAX, INT_MAX, 1, 0, 1}, 
                        {1, INT_MAX, INT_MAX, 1, 0}
                    };
    // Initial Values of Next Hop
    int next[5][5] = {
                        {0, 1, -1, -1, 4}, 
                        {0, 1, 2, -1, -1}, 
                        {-1, 1, 2, 3, -1}, 
                        {-1, -1, 2, 3, 4}, 
                        {0, -1, -1, 3, 4}
                    };
                        
    printf("***************************Initial Routing Table***************************\n");
    for(int i=0; i<n; i++)
    {
        printf("For %c \n", name[i]);
        printf("Destination\t Distance\t Next Hop\n");
        for(int j=0; j<n; j++)
        {
            if(next[i][j]==-1)
            printf("%c\t\t %d\t--\n ", name[j], table[i][j]);
            else
            printf("%c\t\t %d\t\t %c\n ", name[j], table[i][j], name[next[i][j]]);
        }
        printf("\n");
    }
    
    // Sharing routing table with neighbours and updating the distance according to Bellman Ford equation 
    bool flag=true;
    while(flag)
    {
        printf("***************************Round %d of update***************************\n", t+1);
        flag=false;
        // Alphabetically share routing tables of nodes with their neighbours
        // Node     Neighbours    
        // A->      B, E
        // B->      A, C
        // C->      B, D
        // D->      C, E
        // E->      A, D
        for(int i=0; i<n; i++) // For every node
        {
            for(int j=0; j<n; j++) 
            {
                if(table[i][j] == 1)// Find the neighbouring node
                {
                    printf("Sharing distance vector of %c with %c\n", name[i], name[j]);
                    bool change=false;
                    for(int k=0; k<n; k++)// Updating the distance of neighbouring node 'j'
                    {
                        // If any update happens in the routing table then set flag to true 
                        // If no updates we need to stop computing the values
                        // As now the routing table will be stable
                        int distance = table[i][j];
                        if(table[i][k] != INT_MAX && table[j][k] > distance+table[i][k])
                        {
                            change=true;
                            flag=true;
                            table[j][k] = distance+table[i][k];// Updating the distance according to Bellman Ford equation
                            next[j][k]=i;// Updating the next hop
                        }
                    }
                    if(change) // If routing table of 'j' is updated 
                    {
                        printf("Updated routing table of %c upon receiving information from %c is : \n", name[j], name[i]);
                        printf("Destination\t Distance\t Next Hop\n");
                        for(int l=0; l<n; l++)
                        {
                            if(next[j][l]==-1)
                            printf("%c\t\t %d\t--\n ", name[l], table[j][l]);
                            else
                            printf("%c\t\t %d\t\t %c\n ", name[l], table[j][l], name[next[j][l]]);
                        }
                    }
                    else
                    {
                        printf("No update happened in the routing table of %c upon receiving information from %c.", name[j], name[i]);   
                    }
                    printf("\n");
                }
            }
        }
        if(flag) // If flag is true that implies we need to make one more round of update
        t++;
    }
    printf("\nAs no change happened in routing table in round %d. Hence, After %d rounds of update, every node can reach every other node in the network.\n", t+1, t);

    printf("\n***************************Final Updated Routing Table***************************\n");
    for(int i=0; i<n; i++)
    {
        printf("For %c \n", name[i]);
        printf("Destination\t Distance\t Next Hop\n");
        for(int j=0; j<n; j++)
        {
            if(next[i][j]==-1)
            printf("%c\t\t %d\t--\n ", name[j], table[i][j]);
            else
            printf("%c\t\t %d\t\t %c\n ", name[j], table[i][j], name[next[i][j]]);
        }
        printf("\n");
    }
    //***************************Part 2***************************
    // Link BC breaks 
    // A=>0 B=>1 C=>2 D=>3 E=>4
    //      A--------B
    //      |            
    //      |             -----C
    //      |             |
    //      E--------D----
    t=t+1;
    printf("\n***************************Link BC breaks at t = %d***************************\n", t);
    // Update Routing table 
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<n; j++)
        {
            if(i!=1 && i!=2)
            {
                if((j==1 && next[i][j]==2) || (j==2 && next[i][j]==1))
                table[i][j]=INT_MAX;
            }
            else if((i==1 && next[i][j]==2) || (i==2 && next[i][j]==1))
            {
                table[i][j]=INT_MAX;
            }
        }
    }
    // Now again share the distance vector to the neighbouring node and update the routing table.
    // As BC link is broken, it will lead to count to infinity problem
    // So, we need to use split horizon method to resolve it
    // Sharing routing table with neighbours and updating the distance according to Bellman Ford equation 
    flag=true;
    t=t+1;
    while(flag)
    {
        printf("***************************Round %d of update***************************\n", t+1);
        flag=false;
        // Alphabetically share routing tables of nodes with their neighbours
        // Node     Neighbours    
        // A->      B, E
        // B->      A, C
        // C->      B, D
        // D->      C, E
        // E->      A, D
        for(int i=0; i<n; i++) // For every node
        {
            for(int j=0; j<n; j++) 
            {
                if(table[i][j] == 1)// Find the neighbouring node
                {
                    printf("Sharing distance vector of %c with %c\n", name[i], name[j]);
                    bool change=false;
                    for(int k=0; k<n; k++)// Updating the distance of neighbouring node 'j'
                    {
                        // If any update happens in the routing table then set flag to true 
                        // If no updates we need to stop computing the values
                        // As now the routing table will be stable
                        int distance = table[i][j];
                        // Split Horizon 
                        if(next[i][k]==j)
                        continue;

                        if(next[j][k]==i)
                        {
                            if(table[i][k]==INT_MAX)
                            table[j][k]=INT_MAX;
                            else
                            table[j][k]=table[i][k]+distance;
                            change=true;
                            continue;
                        }

                        if(table[i][k] != INT_MAX && table[j][k] > distance+table[i][k])
                        {
                            change=true;
                            flag=true;
                            table[j][k] = distance+table[i][k];// Updating the distance according to Bellman Ford equation
                            next[j][k]=i;// Updating the next hop
                        }
                    }
                    if(change)
                    {
                        printf("Updated routing table of %c upon receiving information from %c is : \n", name[j], name[i]);
                        printf("Destination\t Distance\t Next Hop\n");
                        for(int l=0; l<n; l++)
                        {
                            if(next[j][l]==-1)
                            printf("%c\t\t %d\t--\n ", name[l], table[j][l]);
                            else
                            printf("%c\t\t %d\t\t %c\n ", name[l], table[j][l], name[next[j][l]]);
                        }
                    }
                    else
                    {
                        printf("No update happened in the routing table of %c upon receiving information from %c.", name[j], name[i]);   
                    }
                    printf("\n");
                }
            }
        }
        if(flag) 
        t++;
    }
    printf("\nAs no change happened in routing table in round %d. Hence, After %d rounds of update, every node can reach every other node in the network.\n", t+1, t);

    printf("\n***************************Final Updated Routing Table When Link BC Breaks***************************\n");
    for(int i=0; i<n; i++)
    {
        printf("For %c \n", name[i]);
        printf("Destination\t Distance\t Next Hop\n");
        for(int j=0; j<n; j++)
        {
            if(next[i][j]==-1)
            printf("%c\t\t %d\t--\n ", name[j], table[i][j]);
            else
            printf("%c\t\t %d\t\t %c\n ", name[j], table[i][j], name[next[i][j]]);
        }
        printf("\n");
    }
  
    return 0;
}
