#include "cache.h"

uint32_t CACHE::find_victim(uint32_t cpu, uint64_t instr_id, uint32_t set, const BLOCK *current_set, uint64_t ip, uint64_t full_addr, uint32_t type)
{
    // baseline LRU replacement policy for other caches
    return lru_victim(cpu, instr_id, set, current_set, ip, full_addr, type);
}

void CACHE::update_replacement_state(uint32_t cpu, uint32_t set, uint32_t way, uint64_t full_addr, uint64_t ip, uint64_t victim_addr, uint32_t type, uint8_t hit)
{
    if (type == WRITEBACK)
    {
        if (hit) // wrietback hit does not update LRU state
            return;
    }

    return lru_update(set, way, cpu);
}

uint32_t CACHE::lru_victim(uint32_t cpu, uint64_t instr_id, uint32_t set, const BLOCK *current_set, uint64_t ip, uint64_t full_addr, uint32_t type)
{
    if (cache_type == IS_LLC)
    {
        if (par_way[cpu] != ways_partition[cpu].size())
        {
            uint32_t way = next_way();
            ways_partition[cpu].push_back(way);
            return way;
        }
        else
        {
            // fill invalid line first
            uint32_t i, way;
            for (i = 0; i < ways_partition[cpu].size(); i++)
            {
                way = ways_partition[cpu][i];
                if (block[set][way].valid == false)
                {

                    DP(if (warmup_complete[cpu])
                       {
                           cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " invalid set: " << set << " way: " << way;
                           cout << hex << " address: " << (full_addr >> LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
                           cout << dec << " lru: " << block[set][way].lru << endl;
                       });

                    break;
                }
            }
            // LRU victim
            if (i == ways_partition[cpu].size())
            {
                for (i = 0; i < ways_partition[cpu].size(); i++)
                {
                    way = ways_partition[cpu][i];
                    if (block[set][way].lru == ways_partition[cpu].size() - 1)
                    {

                        DP(if (warmup_complete[cpu])
                           {
                               cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " replace set: " << set << " way: " << way;
                               cout << hex << " address: " << (full_addr >> LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
                               cout << dec << " lru: " << block[set][way].lru << endl;
                           });

                        break;
                    }
                }
            }

            if (i == ways_partition[cpu].size())
            {
                cerr << "[" << NAME << "] " << __func__ << " no victim! set: " << set << endl;
                assert(0);
            }

            return way;
        }
    }
    else
    {
        uint32_t way = 0;

        // fill invalid line first
        for (way = 0; way < NUM_WAY; way++)
        {
            if (block[set][way].valid == false)
            {

                DP(if (warmup_complete[cpu])
                   {
                       cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " invalid set: " << set << " way: " << way;
                       cout << hex << " address: " << (full_addr >> LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
                       cout << dec << " lru: " << block[set][way].lru << endl;
                   });

                break;
            }
        }

        // LRU victim
        if (way == NUM_WAY)
        {
            for (way = 0; way < NUM_WAY; way++)
            {
                if (block[set][way].lru == NUM_WAY - 1)
                {

                    DP(if (warmup_complete[cpu])
                       {
                           cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " replace set: " << set << " way: " << way;
                           cout << hex << " address: " << (full_addr >> LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
                           cout << dec << " lru: " << block[set][way].lru << endl;
                       });

                    break;
                }
            }
        }

        if (way == NUM_WAY)
        {
            cerr << "[" << NAME << "] " << __func__ << " no victim! set: " << set << endl;
            assert(0);
        }

        return way;
    }
}

void CACHE::lru_update(uint32_t set, uint32_t way, uint32_t cpu)
{
    if(cache_type == IS_LLC){
        if(is_hit==true){
            cout<<"\nHIT in way: "<<way<<"\n";
        }
        else{
            cout<<"\nMISS WAY: "<<way<<"\n";
        }
    }
    
    // update lru replacement state
    if (cache_type == IS_LLC)
    {
        if(allocated[way]==false){
            uint32_t i, j;
            for (j = 0; j < ways_partition[cpu].size(); j++)
            {
                i = ways_partition[cpu][j];
                    block[set][i].lru++;
            }
            block[set][way].lru = 0; // promote to the MRU position
            allocated[way]=true;
        }
        else{
            uint32_t i, j;
            for (j = 0; j < ways_partition[cpu].size(); j++)
            {
                i = ways_partition[cpu][j];
                if (block[set][i].lru < block[set][way].lru)
                {
                    block[set][i].lru++;
                }
            }
            block[set][way].lru = 0; // promote to the MRU position
        }
    }
    else
    {
        for (uint32_t i = 0; i < NUM_WAY; i++)
        {
            if (block[set][i].lru < block[set][way].lru)
            {
                block[set][i].lru++;
            }
        }
        block[set][way].lru = 0; // promote to the MRU position
    }
    if(cache_type == IS_LLC){
    for(int i=0;i<ways_partition[cpu].size();i++){
      cout<<block[set][ways_partition[cpu][i]].lru<<" ";
    }
  }
}

void CACHE::replacement_final_stats()
{
}

#ifdef NO_CRC2_COMPILE
void InitReplacementState()
{
}

uint32_t GetVictimInSet(uint32_t cpu, uint32_t set, const BLOCK *current_set, uint64_t PC, uint64_t paddr, uint32_t type)
{
    return 0;
}

void UpdateReplacementState(uint32_t cpu, uint32_t set, uint32_t way, uint64_t paddr, uint64_t PC, uint64_t victim_addr, uint32_t type, uint8_t hit)
{
}

void PrintStats_Heartbeat()
{
}

void PrintStats()
{
}
#endif
