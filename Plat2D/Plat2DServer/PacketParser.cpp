#include "PacketParser.h"
#include "Main.h"

#include "MySQL++/mysql++.h"

void PacketParser(SOCKET sck, const char *Packet)
{
	switch (Packet[0])
	{
	// Movement packet.
	case PacketHeaders.MovementPacket:
		{
			MOVEMENTPACKET *mp = (MOVEMENTPACKET*)Packet;
			cout << "Mov*" << "ID:" << mp->ID << "|" << "MID:" << mp->MapID << "|" << "X:" << mp->XPos << "|" << "Y:" << mp->YPos << endl;
			BroadcastPacket(Packet, sizeof(MOVEMENTPACKET));
		}
		break;

	// Chat packet
	case PacketHeaders.ChatPacket:
		{
			CHATPACKET *cp = (CHATPACKET*)Packet;
			cout << "Chat*"<< cp->ID << "||" << cp->Text << endl;
			BroadcastPacket(Packet, cp->Length);
		}
		break;

	// Login packet
	case PacketHeaders.LoginPacket:
		{
			LOGINPACKET *lp = (LOGINPACKET*)Packet;
			cout << "Login*" << lp->Username << " : " << lp->Password << endl;

			stringstream querystring;
			querystring << "SELECT * FROM (`Users`) WHERE (`Username`='" << lp->Username << "' AND `Password`='" << lp->Password << "') LIMIT 1";
			mysqlpp::Connection con;
			con.connect(0, "localhost", "root", "", 3306);
			con.select_db("moonlightfrost");
			mysqlpp::Query qry(con.query(querystring.str()));
			mysqlpp::StoreQueryResult qr(qry.store());
			con.disconnect();

			if ( qr.num_rows() == 0 )
			{
				lp->UserID = 0;
				send(sck, (char *)lp, sizeof(LOGINPACKET), 0);
			}
			else
			{
				lp->UserID = atol(qr[0]["ID"].c_str());
				send(sck, (char *)lp, sizeof(LOGINPACKET), 0);
			}
		}
		break;

	default:
		{
			printf("WARNING: Recieved packet structure not recognized. Header: %s \n", (int)((char)Packet[0]));
		}
		break;
	}
}