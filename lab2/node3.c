#include <stdio.h>

extern struct rtpkt
{
	int sourceid;       /* id of node sending this pkt */
	int destid;         /* id of router to which pkt being sent
						(must be an immediate neighbor) */
	int mincost[4];     /* min cost to node 0 ... 3 */
};

extern int TRACE;
extern int YES;
extern int NO;

struct distance_table
{
	int costs[4][4];
} dt3;

int neighbors3_len = 2;
int neighbors3[2] = {0, 2};
void tolayer2(struct rtpkt packet);
void printdt3(struct distance_table *dtptr);
int mincost3(int nodei);
/* please complete the following three routines for part 1 */

void rtinit3()
{
	int i = 0;
	int j = 0;
	for (i = 0; i < 4; i++){
		for (j = 0; j < 4; j++) {
			dt3.costs[i][j] = 999;
		}
	}
	dt3.costs[0][0] = 7;
	dt3.costs[2][2] = 2;

}

void rtfinalize3()
{
	printf("node2: final table:\n");
	printdt3(&dt3);
}

void rtupdate3(struct rtpkt *rcvdpkt)
{
	int current_node = 3;
	printf("node3: current table\n");
	printdt3(&dt3);
	int i = 0;

	int update_neighbors = 0;
	//update our table.
	int id = rcvdpkt->sourceid;
	int k = 0;
	int is_neighbor = 0;
	for (i = 0; i < 4; i++) {
		if (i == current_node){
			continue;
		}
		if (dt3.costs[i][rcvdpkt->sourceid] > dt3.costs[rcvdpkt->sourceid][rcvdpkt->sourceid] + rcvdpkt->mincost[i]) {
			update_neighbors = 1;
			dt3.costs[i][rcvdpkt->sourceid] = dt3.costs[rcvdpkt->sourceid][rcvdpkt->sourceid] + rcvdpkt->mincost[i];
		}
	}

	printf("node3: new table\n");
	printdt3(&dt3);
	printf("node3: updating neighbors\n");
	if (update_neighbors == 1) {
		// update neighboring nodes.
		for (i = 0; i < 4; i++) {
			if (i == current_node || i == 1) {
				continue;
			}
			struct rtpkt pkt;
			pkt.sourceid = current_node;
			pkt.destid = i;
			pkt.mincost[0] = mincost3(0);
			pkt.mincost[1] = mincost3(1);
			pkt.mincost[2] = mincost3(2);
			pkt.mincost[3] = mincost3(3);
			tolayer2(pkt);
		}
	}

}
int mincost3(int nodei)
{
	int i = 0;
	int cost = 999;
	for (i = 0; i < neighbors3_len; i++){
		int neighbor = neighbors3[i];
		if (dt3.costs[nodei][neighbor] < cost){
			cost = dt3.costs[nodei][neighbor];
		}
	}
	return cost;
}


void printdt3(struct distance_table *dtptr)

{
	printf("             via     \n");
	printf("   D3 |    0     2 \n");
	printf("  ----|-----------\n");
	printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
	printf("dest 1|  %3d   %3d\n",dtptr->costs[1][0], dtptr->costs[1][2]);
	printf("     2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);

}








