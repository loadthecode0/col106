import requests
import sys
import json

def get_pos(parameters):
    api = "http://text-processing.com/api/tag/"
    response = requests.post(f"{api}", data=parameters)
    if response.status_code == 200:
        response_json = json.loads(response.text)["text"]
        resp_str = json.dumps(response_json)
        if resp_str[-6:-3] in ["NNP", "NNPS"]:
            return "propNoun"
        else:
            return "other"
    else:
        return ""

def get_stem(parameters):
    api = "http://text-processing.com/api/stem/"
    response = requests.post(f"{api}", data=parameters)
    if response.status_code == 200:
        response_json = json.loads(response.text)["text"]
        resp_str = json.dumps(response_json)
        return resp_str[1:-1]
    else:
        return ""

def get_meaningful_stem(stem):
    api = "https://api.datamuse.com/words?sl="
    api += stem
    response = requests.get(f"{api}&max=1")
    if response.status_code == 200:
        response_json = json.loads(response.text)[0]["word"]
        resp_str = json.dumps(response_json)
        return resp_str[1:-1]
    else:
        return ""

def get_synonyms(stem, nbr="", num=0):
    api = "https://api.datamuse.com/words?rel_syn="
    api += stem
    api += f"&max{num}"
    if nbr != "":
        api += "&topics=" + nbr
    response = requests.get(f"{api}")  # include &topics={neighbour words}?
    if response.status_code == 200:
        response_json = json.loads(response.text)[:num]
        myList = []
        for x in range(len(response_json)):
            response_json_x = json.loads(response.text)[x]["word"]
            myList.append(response_json_x)
        return myList
    else:
        return []



if __name__ == '__main__':

    # # Read the raw query tokens from the command line
    parsedCmd = sys.argv[1:]
    bagCap = 2*len(parsedCmd)  # 4*no.of raw tokens

    # calculate total of frequencies for proportion calc
    weightDict = {}
    print(parsedCmd)
    for i in range(0, len(parsedCmd), 2):
        rawToken = parsedCmd[i]
        rawFreq = parsedCmd[i+1]
        if rawFreq != 0:
            weightDict[rawToken] = rawFreq

    print("weightDict", weightDict)

    NNPList = []
    otherPosList = []
    relWtDict = {}

    filename = 'query1.txt'  # contains processed tokens
    with open(filename, 'w') as f:
        for i in range(0, len(parsedCmd), 2):
            rawToken = parsedCmd[i]
            paramDict = {"text": rawToken}
            pos = get_pos(paramDict)
            if pos == "propNoun":
                NNPList.append(rawToken)
                f.write(f"{rawToken}!\n")  # can modify marker for proper noun
            else:
                otherPosList.append(rawToken)
                f.write(f"{rawToken}\n")

        f.write("\n")
        totalInvFreq = sum((1/int(weightDict[i]) for i in otherPosList))

        for i in range(len(otherPosList)):
            token = otherPosList[i]
            if (len(otherPosList) == 1):
                neighbor = ""
            elif (i == 0):
                neighbor = otherPosList[i+1]
            elif (i == len(otherPosList) - 1):
                neighbor = otherPosList[i-1]
            else:
                neighbor = otherPosList[i+1]
                otherNeighbor = otherPosList[i-1]
                if (weightDict[neighbor] > weightDict[otherNeighbor]):
                    neighbor = otherNeighbor

            rawFreq = weightDict[token]
            relWtDict[token] = round(
                (1/int(rawFreq))*bagCap/(totalInvFreq))  # round off
            paramDict = {"text": token}
            stem = get_stem(paramDict)
            print(stem)
            meaningfulStem = get_meaningful_stem(stem)
            print(meaningfulStem)
            if (meaningfulStem == token):
                synList1 = get_synonyms(token, neighbor, (relWtDict[token]))
                synSet = set(synList1)
            else:
                synList1 = get_synonyms(
                    token, neighbor, round(relWtDict[token]/2))
                synSet = set(synList1)
                f.write(f"{meaningfulStem}\n")
                synList2 = get_synonyms(
                    meaningfulStem, neighbor, round(relWtDict[token]/2))
                set2 = set(synList2)
                synSet = synSet.union(set2)

            for x in synSet:
                if (x != token):
                    f.write(f"{x}\n")

            f.write("\n")

    print(relWtDict)
    print(NNPList)
    print(otherPosList)

    print("completed api calls")

    # modifications to try: ----UPDATE: tried all
    # playing around with topics tag
    # relative proportions
    # relative importance of returned synonyms in paragraph search to be considered
