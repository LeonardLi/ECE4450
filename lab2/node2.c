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
} dt2;


int current_node = 2;

int neighbors2_len = 3;
int neighbors2[3] = {0, 1, 3};

void tolayer2(struct rtpkt packet);
void printdt2(struct distance_table *dtptr);

/* please complete the following three routines for part 1 */

void rtinit2()
{

	int i = 0;
	int j = 0;
	for (i = 0; i < 4; i++){
		for (j = 0; j < 4; j++) {
			dt2.costs[i][j] = 999;
		}
	}
	dt2.costs[3][3] = 2;
	dt2.costs[0][0] = 3;
	dt2.costs[1][1] = 1;
}

void rtfinalize2()
{
	printf("node2: final table:\n");
	printdt2(&dt2);
}

void rtupdate2(struct rtpkt *rcvdpkt)
{
	int current_node = 2;
	printf("node2: current table\n");
	printdt2(&dt2);
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

		if (dt2.costs[i][rcvdpkt->sourceid] > dt2.costs[rcvdpkt->sourceid][rcvdpkt->sourceid] + rcvdpkt->mincost[i]) {
			update_neighbors = 1;
			/*      printf("i: %i\n", i);
            printf("rcvdpkt->sourceid: %i\n", rcvdpkt->sourceid);
            printf("dt2.costs[i][rcvdpkt->sourceid]: %i\n", dt2.costs[i][rcvdpkt->sourceid]);
            printf("dt2.costs[rcvdpkt->sourceid][rcvdpkt->sourceid]: %i\n", dt2.costs[rcvdpkt->sourceid][rcvdpkt->sourceid]);
            printf("mincost[i]: %i\n", rcvdpkt->mincost[i]);*/
			dt2.costs[i][rcvdpkt->sourceid] = dt2.costs[rcvdpkt->sourceid][rcvdpkt->sourceid] + rcvdpkt->mincost[i];
		}
	}

	printf("node2: new table\n");
	printdt2(&dt2);
	printf("node2: updating neighbors\n");
	if (update_neighbors == 1) {
		// update neighboring nodes.
		for (i = 0; i < 4; i++) {
			if (i == current_node) {
				continue;
			}
			struct rtpkt pkt;
			pkt.sourceid = current_node;
			pkt.destid = i;
			pkt.mincost[0] = mincost2(0);
			pkt.mincost[1] = mincost2(1);
			pkt.mincost[2] = mincost2(2);
			pkt.mincost[3] = mincost2(3);
			tolayer2(pkt);
		}
	}
}

int mincost2(int nodei)
{
	int i = 0;
	int cost = 999;
	for (i = 0; i < neighbors2_len; i++){
		int neighbor = neighbors2[i];
		if (dt2.costs[nodei][neighbor] < cost){
			cost = dt2.costs[nodei][neighbor];
		}
	}
	return cost;
}



void printdt2(struct distance_table *dtptr)

{
	printf("                via     \n");
	printf("   D2 |    0     1    3 \n");
	printf("  ----|-----------------\n");
	printf("     0|  %3d   %3d   %3d\n",dtptr->costs[0][0],
		   dtptr->costs[0][1],dtptr->costs[0][3]);
	printf("dest 1|  %3d   %3d   %3d\n",dtptr->costs[1][0],
		   dtptr->costs[1][1],dtptr->costs[1][3]);
	printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][0],
		   dtptr->costs[3][1],dtptr->costs[3][3]);
}




