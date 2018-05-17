/*
 * =========================================================================================
 * Name        : processData.cpp
 * Description : student code for Assignment 1 - Data structures and Algorithms - Spring 2018
 * =========================================================================================
 */
#include "requestLib.h"
#include "dbLib.h"

bool initVGlobalData(void** pGData) {
	*pGData = NULL;
	return true;
}

void releaseVGlobalData(void* pGData) {
	delete pGData;
}

void createDatabaseFromList(void *&pGData, L1List<VRecord> &recList) {
	if (pGData) return;
	pGData = new L1List<L2Item<VRecord>>();
	L1List<L2Item<VRecord>> *pList = (L1List<L2Item<VRecord>> *)pGData;
	L1Item<VRecord> *pCurrent = recList.getHead();
	int iIndex = -1;
	while (pCurrent) {
		L2Item<VRecord> *pNew = new L2Item<VRecord>(pCurrent->data.id, &recList);
		if (!pList->find(*pNew, iIndex)) {
			pList->insertHead(*pNew);
		}
		pCurrent = pCurrent->pNext;
	}
	pList->reverse();
}

void getRequest(VRequest &request, int &code, char *&id) {
	string sCode = "";
	for (int i = 0; i < 3; i++) sCode += request.code[i];
	string sList[27] = {
		"CNV", "VFF", "VFL", "VFY", "VFX", "VLY", "VLX",
		"VFT", "VLT", "VCR", "VCL", "VMT", "VFS", "VLS",
		"VMS", "VAS", "MST", "CNR", "MRV", "LRV", "MTV",
		"MVV", "CNS", "CAS", "LPV", "SPV", "RVR"
	};
	code = find(sList, sList + 27, sCode) - sList;
	strcpy(id, request.code + 3);
}

L1List<VRecord> *targetList(L1List<L2Item<VRecord>> *pList, char *id) {
	L1List<VRecord> *pResult = new L1List<VRecord>();
	L1Item<L2Item<VRecord>> *pCurrent = pList->getHead();
	while (pCurrent && strcmp(pCurrent->data.cId, id) != 0) {
		pCurrent = pCurrent->pNext;
	}
	if (pCurrent) pResult = pCurrent->data.pList;
	return pResult;
}

char *vehicleFirstFind(L1List<VRecord>& recList) {
	if (recList.getSize() == 0) {
		throw "VFF: Failed";
	}
	return recList.at(0).id;
}

char *vehicleLastFind(L1List<VRecord>& recList) {
	if (recList.getSize() == 0) {
		throw "VLF: Failed";
	}
	return recList.at(recList.getSize()-1).id;
}

double vehicleFirstY(L1List<L2Item<VRecord>> *pList, char *id) {
	L1List<VRecord> *pTarget = targetList(pList, id);
	if (pTarget->getSize() == 0) {
		throw "VFY " + string(id) + ": Failed";
	}
	return pTarget->at(0).y;
}

double vehicleFirstX(L1List<L2Item<VRecord>> *pList, char *id) {
	L1List<VRecord> *pTarget = targetList(pList, id);
	if (pTarget->getSize() == 0) {
		throw "VFX " + string(id) + ": Failed";
	}
	return pTarget->at(0).x;
}

double vehicleLastY(L1List<L2Item<VRecord>> *pList, char *id) {
	L1List<VRecord> *pTarget = targetList(pList, id);
	if (pTarget->getSize() == 0) {
		throw "VLY " + string(id) + ": Failed";
	}
	return pTarget->at(pTarget->getSize() - 1).y;
}

double vehicleLastX(L1List<L2Item<VRecord>> *pList, char *id) {
	L1List<VRecord> *pTarget = targetList(pList, id);
	if (pTarget->getSize() == 0) {
		throw "VLX " + string(id) + ": Failed";
	}
	return pTarget->at(pTarget->getSize() - 1).x;
}

time_t vehicleFirstTime(L1List<L2Item<VRecord>> *pList, char *id) {
	L1List<VRecord> *pTarget = targetList(pList, id);
	if (pTarget->getSize() == 0) {
		throw "VFT " + string(id) + ": Failed";
	}
	return pTarget->at(0).timestamp;
}

time_t vehicleLastTime(L1List<L2Item<VRecord>> *pList, char *id) {
	L1List<VRecord> *pTarget = targetList(pList, id);
	if (pTarget->getSize() == 0) {
		throw "VLT " + string(id) + ": Failed";
	}
	return pTarget->at(pTarget->getSize() - 1).timestamp;
}

int vehicleCountRecords(L1List<L2Item<VRecord>> *pList, char *id) {
	L1List<VRecord> *pTarget = targetList(pList, id);
	return pTarget->getSize();
}

double vehicleCountLength(L1List<L2Item<VRecord>> *pList, char *id) {
	L1List<VRecord> *pTarget = targetList(pList, id);
	double dResult = 0.0;
	if (pTarget->getSize() == 0) {
		throw "VCL " + string(id) + ": Failed";
	}
	if (pTarget->getSize() == 1) return dResult;
	else {
		L1Item<VRecord> *pCurrent = pTarget->getHead();
		L1Item<VRecord> *pNext = pTarget->getHead()->pNext;
		while (pNext) {
			dResult += distanceVR(pCurrent->data.y, pCurrent->data.x, pNext->data.y, pNext->data.x);
			pCurrent = pCurrent->pNext;
			pNext = pNext->pNext;
		}
		return dResult;
	}
}

int vehicleMoveTime(L1List<L2Item<VRecord>> *pList, char *id) {
	L1List<VRecord> *pTarget = targetList(pList, id);
	if (pTarget->getSize() == 0) {
		throw "VMT " + string(id) + ": Failed";
	}
	if (pTarget->getSize() == 1) return 0;
	else {
		return pTarget->at(pTarget->getSize() - 1).timestamp - pTarget->at(0).timestamp;
	}
}

L1List<VRecord> *vehicleStopPointList(L1List<L2Item<VRecord>> *pList, char *id) {
	L1List<VRecord> *pResult = new L1List<VRecord>();
	L1List<VRecord> *pTarget = targetList(pList, id);
	if (pTarget->getSize() == 0) {
		return pResult;
	}
	else if (pTarget->getSize() == 1) {
		pResult->insertHead(pTarget->getHead()->data);
		return pResult;
	}
	else {
		L1Item<VRecord> *pCurrent = pTarget->getHead();
		L1Item<VRecord> *pNext = pTarget->getHead()->pNext;
		while (pNext) {
			double dResult = distanceVR(pCurrent->data.y, pCurrent->data.x, pNext->data.y, pNext->data.x);
			if (dResult == 0) pResult->insertHead(pCurrent->data);
			pCurrent = pCurrent->pNext;
			pNext = pNext->pNext;
		}
		pResult->reverse();
		return pResult;
	}
}

VRecord vehicleFirstStop(L1List<L2Item<VRecord>> *pList, char *id) {
	L1List<VRecord> *pTarget = targetList(pList, id);
	if (pTarget->getSize() == 0) {
		throw "VFS " + string(id) + ": Failed";
	}
	if (pTarget->getSize() == 1) return pTarget->getHead()->data;
	else {
		L1List<VRecord> *pStop = vehicleStopPointList(pList, id);
		return pStop->at(0);
	}
}

VRecord vehicleLastStop(L1List<L2Item<VRecord>> *pList, char *id) {
	L1List<VRecord> *pTarget = targetList(pList, id);
	if (pTarget->getSize() == 0) {
		throw "VLS " + string(id) + ": Failed";
	}
	if (pTarget->getSize() == 1) return pTarget->getHead()->data;
	else {
		L1List<VRecord> *pStop = vehicleStopPointList(pList, id);
		return pStop->at(pStop->getSize()-1);
	}
}

int vehicleMostStop(L1List<L2Item<VRecord>> *pList, char *id) {
	L1List<VRecord> *pTarget = targetList(pList, id);
	if (pTarget->getSize() == 0) {
		throw "VMS " + string(id) + ": Failed";
	}
	if (pTarget->getSize() == 1) {
		return 0;
	}
	else {
		int iMax = 0;
		L1Item<VRecord> *pCurrent = pTarget->getHead();
		L1Item<VRecord> *pNext = pTarget->getHead()->pNext;
		while (pNext) {
			int iTime = 0;
			while (pNext && distanceVR(pCurrent->data.y, pCurrent->data.x, pNext->data.y, pNext->data.x) == 0) {
				if (pNext->pNext == NULL || distanceVR(pCurrent->data.y, pCurrent->data.x, pNext->pNext->data.y, pNext->pNext->data.x) != 0) {
					iTime = pNext->data.timestamp - pCurrent->data.timestamp;
				}
				pNext = pNext->pNext;
			}
			if (iTime > iMax) iMax = iTime;
			pCurrent = pNext;
			if (pNext != NULL) pNext = pNext->pNext;
		}
		return iMax;
	}
}

double vehicleVelocity(L1List<L2Item<VRecord>> *pList, char *id) {
	return vehicleCountLength(pList, id) / vehicleMoveTime(pList, id);
}

double vehicleAverageStreet(L1List<L2Item<VRecord>> *pList, char *id) {
	L1List<VRecord> *pTarget = targetList(pList, id);
	if (pTarget->getSize() == 0) {
		throw "VAS " + string(id) + ": Failed";
	}
	if (pTarget->getSize() == 1) return 0;
	else return vehicleCountLength(pList, id) / (pList->getSize() - 1);
}

int mostStopTime(L1List<L2Item<VRecord>> *pList) {
	if (pList->getSize() == 0) {
		throw "MST: Failed";
	}
	if (pList->getSize() == 1) return vehicleMostStop(pList, pList->getHead()->data.cId);
	else {
		L1Item<L2Item<VRecord>> *pCurrent = pList->getHead();
		int iMax = 0;
		while (pCurrent) {
			if (vehicleMostStop(pList, pCurrent->data.cId) > iMax) iMax = vehicleMostStop(pList, pCurrent->data.cId);
			pCurrent = pCurrent->pNext;
		}
		return iMax;
	}
}

int countNumberRecord(L1List<VRecord>& recList) {
	return recList.getSize();
}

char *mostRecordVehicle(L1List<L2Item<VRecord>> *pList) {
	if (pList->getSize() == 0) {
		throw "MRV: Failed";
	}
	if (pList->getSize() == 1) return pList->at(0).cId;
	else {
		L1Item<L2Item<VRecord>> *pCurrent = pList->getHead();
		L1Item<L2Item<VRecord>> *pMax = pList->getHead();
		while (pCurrent) {
			if (vehicleCountRecords(pList, pCurrent->data.cId) > vehicleCountRecords(pList, pMax->data.cId)) pMax = pCurrent;
			pCurrent = pCurrent->pNext;
		}
		return pMax->data.cId;
	}
}

char *leastRecordVehicle(L1List<L2Item<VRecord>> *pList) {
	if (pList->getSize() == 0) {
		throw "LRV: Failed";
	}
	if (pList->getSize() == 1) return pList->at(0).cId;
	else {
		L1Item<L2Item<VRecord>> *pCurrent = pList->getHead();
		L1Item<L2Item<VRecord>> *pMin = pList->getHead();
		while (pCurrent) {
			if (vehicleCountRecords(pList, pCurrent->data.cId) < vehicleCountRecords(pList, pMin->data.cId)) pMin = pCurrent;
			pCurrent = pCurrent->pNext;
		}
		return pMin->data.cId;
	}
}

char *mostTimeVehicle(L1List<L2Item<VRecord>> *pList) {
	if (pList->getSize() == 0) {
		throw "MTV: Failed";
	}
	if (pList->getSize() == 1) return pList->at(0).cId;
	else {
		L1Item<L2Item<VRecord>> *pCurrent = pList->getHead();
		L1Item<L2Item<VRecord>> *pMax = pList->getHead();
		while (pCurrent) {
			if (vehicleMoveTime(pList, pCurrent->data.cId) > vehicleMoveTime(pList, pMax->data.cId)) pMax = pCurrent;
			pCurrent = pCurrent->pNext;
		}
		return pMax->data.cId;
	}
}

char *mostVelocityVehicle(L1List<L2Item<VRecord>> *pList) {
	if (pList->getSize() == 0) {
		throw "MVV: Failed";
	}
	if (pList->getSize() == 1) return pList->at(0).cId;
	else {
		L1Item<L2Item<VRecord>> *pCurrent = pList->getHead();
		L1Item<L2Item<VRecord>> *pMax = pList->getHead();
		while (pCurrent) {
			if (vehicleVelocity(pList, pCurrent->data.cId) > vehicleVelocity(pList, pMax->data.cId)) pMax = pCurrent;
			pCurrent = pCurrent->pNext;
		}
		return pMax->data.cId;
	}
}

int countNeverStop(L1List<L2Item<VRecord>> *pList) {
	if (pList->getSize() == 0) {
		throw "CNS: Failed";
	}
	L1Item<L2Item<VRecord>> *pCurrent = pList->getHead();
	int iCount = 0;
	while (pCurrent) {
		L1List<VRecord> *pStop = vehicleStopPointList(pList, pCurrent->data.cId);
		if (pStop->getSize() == 0) iCount++;
		pCurrent = pCurrent->pNext;
	}
	return iCount;
}

double countAverageStreet(L1List<L2Item<VRecord>> *pList) {
	if (pList->getSize() == 0) {
		throw "CAS: Failed";
	}
	double averageStreet = 0.0;
	L1Item<L2Item<VRecord>> *pCurrent = pList->getHead();
	while (pCurrent) {
		averageStreet += vehicleCountLength(pList, pCurrent->data.cId);
		pCurrent = pCurrent->pNext;
	}
	averageStreet /= pList->getSize();
	return averageStreet;
}

char *longestPerimeterVehicle(L1List<L2Item<VRecord>> *pList) {
	if (pList->getSize() == 0) {
		throw "LPV: Failed";
	}
	if (pList->getSize() == 1) return pList->at(0).cId;
	else {
		L1Item<L2Item<VRecord>> *pCurrent = pList->getHead();
		L1Item<L2Item<VRecord>> *pMax = pList->getHead();
		while (pCurrent) {
			if (vehicleCountLength(pList, pCurrent->data.cId) > vehicleCountLength(pList, pMax->data.cId)) pMax = pCurrent;
			pCurrent = pCurrent->pNext;
		}
		return pMax->data.cId;
	}
}

char *shortestPerimeterVehicle(L1List<L2Item<VRecord>> *pList) {
	if (pList->getSize() == 0) {
		throw "SPV: Failed";
	}
	if (pList->getSize() == 1) return pList->at(0).cId;
	else {
		L1Item<L2Item<VRecord>> *pCurrent = pList->getHead();
		L1Item<L2Item<VRecord>> *pMin = pList->getHead();
		while (pCurrent) {
			if (vehicleCountLength(pList, pCurrent->data.cId) < vehicleCountLength(pList, pMin->data.cId)) pMin = pCurrent;
			pCurrent = pCurrent->pNext;
		}
		return pMin->data.cId;
	}
}

void recycleVehicleRecord(L1List<VRecord> &recList, void *&pGData, char *id) {
	L1List<L2Item<VRecord>> *pList = (L1List<L2Item<VRecord>> *)pGData;
	L1List<VRecord> *pTarget = targetList(pList, id);
	if (pTarget->getSize() == 0) {
		throw "RVR " + string(id) + ": Failed";
	}
	L1Item<VRecord> *pCurrent = recList.getHead();
	int iIndex = 0;
	while (pCurrent) {
		L1Item<VRecord> *pNext = pCurrent->pNext;
		if (strcmp(pCurrent->data.id, id) == 0) {
			recList.remove(iIndex);
		}
		else {
			iIndex++;
		}
		pCurrent = pNext;
	}
	iIndex = 0;
	L1Item<L2Item<VRecord>> *pItem = pList->getHead();
	while (pItem && strcmp(pItem->data.cId, id) != 0) {
		pItem = pItem->pNext;
		iIndex++;
	}
	pList->remove(iIndex);
	cout << "RVR " << string(id) << ": Success" << endl;
}

bool processRequest(VRequest& request, L1List<VRecord>& recList, void *pGData) {
	try {
		int iCode = -1;
		char *id = new char[ID_MAX_LENGTH];
		createDatabaseFromList(pGData, recList);
		getRequest(request, iCode, id);
		L1List<L2Item<VRecord>> *pList = (L1List<L2Item<VRecord>> *)pGData;
		switch (iCode) {
		case 0:
			cout << pList->getSize() << endl;
			break;
		case 1:
			cout << vehicleFirstFind(recList) << endl;
			break;
		case 2:
			cout << vehicleLastFind(recList) << endl;
			break;
		case 3:
			cout << vehicleFirstY(pList, id) << endl;
			break;
		case 4:
			cout << vehicleFirstX(pList, id) << endl;
			break;
		case 5:
			cout << vehicleLastY(pList, id) << endl;
			break;
		case 6:
			cout << vehicleLastX(pList, id) << endl;
			break;
		case 7:
			cout << vehicleFirstTime(pList, id) << endl;
			break;
		case 8:
			cout << vehicleLastTime(pList, id) << endl;
			break;
		case 9:
			cout << vehicleCountRecords(pList, id) << endl;
			break;
		case 10:
			cout << vehicleCountLength(pList, id) << endl;
			break;
		case 11:
			cout << vehicleMoveTime(pList, id) << endl;
			break;
		case 12:
			cout << "(" << vehicleFirstStop(pList, id).x << ", " << vehicleFirstStop(pList, id).y << ")" << endl;
			break;
		case 13:
			cout << "(" << vehicleLastStop(pList, id).x << ", " << vehicleLastStop(pList, id).y << ")" << endl;
			break;
		case 14:
			cout << vehicleMostStop(pList, id) << endl;
			break;
		case 15:
			cout << vehicleAverageStreet(pList, id) << endl;
			break;
		case 16:
			cout << mostStopTime(pList) << endl;
			break;
		case 17:
			cout << countNumberRecord(recList) << endl;
			break;
		case 18:
			cout << mostRecordVehicle(pList) << endl;
			break;
		case 19:
			cout << leastRecordVehicle(pList) << endl;
			break;
		case 20:
			cout << mostTimeVehicle(pList) << endl;
			break;
		case 21:
			cout << mostVelocityVehicle(pList) << endl;
			break;
		case 22:
			cout << countNeverStop(pList) << endl;
			break;
		case 23:
			cout << countAverageStreet(pList) << endl;
			break;
		case 24:
			cout << longestPerimeterVehicle(pList) << endl;
			break;
		case 25:
			cout << shortestPerimeterVehicle(pList) << endl;
			break;
		case 26:
			recycleVehicleRecord(recList, pGData, id);
			break;
		default:
			break;
		}
	}
	catch (string exception) {
		cout << exception << endl;
	}
	return true;
}

