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
} dt0;


int neighbors0_len = 3;
int neighbors0[3] = {1, 2, 3};
void printdt0(struct distance_table *dtptr);
int mincost0(int nodei);

void tolayer2(struct rtpkt packet);

/* please complete the following three routines for part 1 */

void rtinit0()
{
	printf("initing node0\n");
	dt0.costs[0][0] = 0;
	dt0.costs[1][1] = 1;
	dt0.costs[2][2] = 3;
	dt0.costs[3][3] = 7;
	int i = 0;
	int j = 0;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if(i != j) {
				dt0.costs[i][j] = 999;
			}
		}
	}

	printf("printing initial node0\n");
	printdt0(&dt0);
	// update neighbor nodes
	for (i = 1; i < 4; i++) {
		struct rtpkt pkt;
		pkt.sourceid = 0;
		pkt.destid = i;
		pkt.mincost[0] = dt0.costs[0][0];
		pkt.mincost[1] = dt0.costs[1][1];
		pkt.mincost[2] = dt0.costs[2][2];
		pkt.mincost[3] = dt0.costs[3][3];
		tolayer2(pkt);

	}

}

void rtfinalize0()
{
	printf("node0: final table:\n");
	printdt0(&dt0);
}

void rtupdate0(struct rtpkt *rcvdpkt)
{
	int current_node = 0;
	printf("node0: current table\n");
	printdt0(&dt0);
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
		if (dt0.costs[i][rcvdpkt->sourceid] > dt0.costs[rcvdpkt->sourceid][rcvdpkt->sourceid] + rcvdpkt->mincost[i]) {
			update_neighbors = 1;//need update
			dt0.costs[i][rcvdpkt->sourceid] = dt0.costs[rcvdpkt->sourceid][rcvdpkt->sourceid] + rcvdpkt->mincost[i];
		}
	}

	printf("node0: new table\n");
	printdt0(&dt0);
	printf("node0: updating neighbors\n");
	if (update_neighbors == 1) {
		// update neighboring nodes.
		for (i = 0; i < 4; i++) {
			if (i == current_node) {
				continue;
			}
			struct rtpkt pkt;
			pkt.sourceid = current_node;
			pkt.destid = i;
			pkt.mincost[0] = mincost0(0);
			pkt.mincost[1] = mincost0(1);
			pkt.mincost[2] = mincost0(2);
			pkt.mincost[3] = mincost0(3);
			tolayer2(pkt);
		}
	}

}


int mincost0(int nodei)
{
	int i = 0;
	int cost = 999;
	for (i = 0; i < neighbors0_len; i++){
		int neighbor = neighbors0[i];
		if (dt0.costs[nodei][neighbor] < cost){
			cost = dt0.costs[nodei][neighbor];
		}
	}
	return cost;
}

void printdt0(struct distance_table *dtptr)
{
	printf("                via     \n");
	printf("   D0 |    1     2    3 \n");
	printf("  ----|-----------------\n");
	printf("     1|  %3d   %3d   %3d\n",dtptr->costs[1][1],
		   dtptr->costs[1][2],dtptr->costs[1][3]);
	printf("dest 2|  %3d   %3d   %3d\n",dtptr->costs[2][1],
		   dtptr->costs[2][2],dtptr->costs[2][3]);
	printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][1],
		   dtptr->costs[3][2],dtptr->costs[3][3]);
}
/* please complete the following routine for part 2 */

void rtlinkhandler0(int linkid, int newcost)
{
	dt0.costs[linkid][linkid] = newcost;
	int i;
	// update neighbor nodes
	for (i = 1; i < 4; i++) {
		struct rtpkt pkt;
		pkt.sourceid = i;
		pkt.mincost[0] = dt0.costs[0][0];
		pkt.mincost[1] = dt0.costs[1][1];
		pkt.mincost[2] = dt0.costs[2][2];
		pkt.mincost[3] = dt0.costs[3][3];
		tolayer2(pkt);

	}
}
