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
int connectcosts1[4] = { 1,  0,  1, 999 };
int neighbors1_len = 2;
int neighbors1[2] = {0, 2};

struct distance_table
{
	int costs[4][4];
} dt1;
void tolayer2(struct rtpkt packet);
void printdt1(struct distance_table *dtptr);
int mincost1(int nodei);


/* please complete the following three routines for part 1 */

void rtinit1()
{
	int i = 0;
	int j = 0;
	for (i = 0; i < 4; i++){
		for (j = 0; j < 4; j++) {
			dt1.costs[i][j] = 999;
		}
	}

	dt1.costs[0][0] = 1;
	dt1.costs[2][2] = 1;
}

void rtfinalize1()
{
	printf("node1: final table:\n");
	printdt1(&dt1);
}

void rtupdate1(struct rtpkt *rcvdpkt)
{
	int current_node = 1;
	printf("node1: current table\n");
	printdt1(&dt1);
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

		if (dt1.costs[i][rcvdpkt->sourceid] > dt1.costs[rcvdpkt->sourceid][rcvdpkt->sourceid] + rcvdpkt->mincost[i]) {
			update_neighbors = 1;
			dt1.costs[i][rcvdpkt->sourceid] = dt1.costs[rcvdpkt->sourceid][rcvdpkt->sourceid] + rcvdpkt->mincost[i];
		}
	}

	printf("node1: new table\n");
	printdt1(&dt1);
	printf("node1: updating neighbors\n");
	if (update_neighbors == 1) {
		// update neighboring nodes.
		for (i = 0; i < 4; i++) {
			if (i == current_node || i == 3) {
				continue;
			}
			struct rtpkt pkt;
			pkt.sourceid = current_node;
			pkt.destid = i;
			pkt.mincost[0] = mincost1(0);
			pkt.mincost[1] = mincost1(1);
			pkt.mincost[2] = mincost1(2);
			pkt.mincost[3] = mincost1(3);
			tolayer2(pkt);
		}
	}
}

int mincost1(int nodei)
{
	int i = 0;
	int cost = 999;
	for (i = 0; i < neighbors1_len; i++){
		int neighbor = neighbors1[i];
		if (dt1.costs[nodei][neighbor] < cost){
			cost = dt1.costs[nodei][neighbor];
		}
	}
	return cost;
}

void printdt1(struct distance_table *dtptr)
{
	printf("             via   \n");
	printf("   D1 |    0     2 \n");
	printf("  ----|-----------\n");
	printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
	printf("dest 2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);
	printf("     3|  %3d   %3d\n",dtptr->costs[3][0], dtptr->costs[3][2]);

}
/* please complete the following routine for part 2 */

void rtlinkhandler1(int linkid, int newcost)
{
    dt1.costs[linkid][linkid] = newcost;
    int i;
    // update neighbor nodes
    for (i = 1; i < 4; i++) {
        struct rtpkt pkt;
        pkt.sourceid = i;
        pkt.mincost[0] = dt1.costs[0][0];
        pkt.mincost[1] = dt1.costs[1][1];
        pkt.mincost[2] = dt1.costs[2][2];
        pkt.mincost[3] = dt1.costs[3][3];
        tolayer2(pkt);

    }
}
