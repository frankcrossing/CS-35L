# Keep the function signature,
# but replace its body with your implementation

import os
import sys
import zlib


def isParent(cwd):
    git = False
    name = ".git"
    fileList = os.listdir(cwd)
    
    for files in fileList:
        if name in files:
            git = True
            break

    return git

class CommitNode:
    def __init__(self, commit_hash):
        """
        :type commit_hash: str
        """
        self.commit_hash = commit_hash
        self.parents = []
        self.children = set()
        self.branch = []

def buildCommit(branch):
    
    commitList = []
 
    f = open('.git/refs/heads/' + branch)
    content = f.read()
    content = content[:-1]
    f.close()

    root = True
    oldContent = ""

    counter = 0

    first = True

    while (True):
        commit = CommitNode(content)
    
        if (root == False):
            commit.children.add(oldContent)

        f = open('.git/objects/' + content[0:2] + '/' + content[2:], 'rb')
        message = f.read()
        message = zlib.decompress(message)
        message = message.decode("ascii")
        f.close()
    
        if (len(message.split("parent ", 1)) == 1):
            commitList.insert(counter, commit)
            break
        
        messageParents = message

        while messageParents.split("parent ", 1)[0] != messageParents:
            messageParents = messageParents.split("parent ", 1)[1]
            actualParents = messageParents.split("\n", 1)[0]
            commit.parents.insert(len(commit.parents), actualParents)
            messageParents = messageParents.split("\n", 1)[1]
        
        oldContent = content
        
        content = commit.parents[0]

        root = False

        commitList.insert(counter, commit)
        counter = counter + 1
    
    return commitList
 
def recurseSort(commit, finalStack, all_commits, all_names):
    tempStack = []
    tempStack.append(commit)
    
    while (len(tempStack) != 0):
        added = False

        topElement = tempStack[-1]
        if topElement in finalStack:
            tempStack.pop()
            continue
        else:
            for i in topElement.parents:
                index = all_names.index(i)
                if (all_commits[index] not in finalStack):
                    tempStack.append(all_commits[index])
                    added = True
            if (added == False):
                tempStack.pop()
                finalStack.insert(0,topElement)

def topologicalSorting(root_commits, unorderedNames):
    
    finalStack = []
    
    for root in root_commits:
        recurseSort(root, finalStack, root_commits, unorderedNames)

    return finalStack

def printing(commit, index, orderedList, commitNames, visited, lastPath):
    
    toBeReturned = ""
    if visited[index] == True or len(commit.parents) == 0:
        if lastPath == True:
            print(commit.commit_hash + '=')
        else:
            print(commit.commit_hash + '=\n')
            print('=')
    
    else:
        string = commit.commit_hash
        for i in commit.branch:
            string = string + " " + i
        print(string)

    if visited[index] == False:
        visited[index] = True
        for node in commit.parents:
            nextIndex = commitNames.index(node)
            toBeReturned = printing(orderedList[nextIndex], nextIndex, orderedList, commitNames, visited, lastPath)

    return toBeReturned

def topo_order_commits():
    cwd = os.getcwd()
    git = False
    
    while (cwd != ''):
        git = isParent(cwd)
        if (git == True):
            break
        os.chdir("..")
        cwd = os.getcwd()
    
    if (git == False):
        git = isParent(cwd)
    
    if (git == False):
        sys.stderr.write("Not a git repository." + "\n")
        exit(1)

    branches = os.listdir('.git/refs/heads')
    root_commits = []
    commitNames = []

    for branch in branches:
        commitList = buildCommit(branch)
        for commit in commitList:
            if (commit.commit_hash not in commitNames):
                commitNames.insert(len(commitNames), commit.commit_hash)
                root_commits.insert(len(root_commits), commit)
            else:
                node = root_commits[commitNames.index(commit.commit_hash)]
                for children in commit.children:
                    if children not in node.children:
                        node.children.add(children)
                for parent in commit.parents:
                    if parent not in node.parents:
                        node.parents.add(parent)

    for commit in root_commits:
        for branch in branches:
            f = open('.git/refs/heads/' + branch)
            stuff = f.read()
            stuff = stuff[:-1]
            f.close()
            if (commit.commit_hash == stuff): 
                commit.branch.insert(len(commit.branch), branch)
                commit.branch.sort()
    unorderedNames = []
    
    for node in root_commits:
        unorderedNames.insert(len(unorderedNames), node.commit_hash)
    
    orderedList = topologicalSorting(root_commits, unorderedNames)

    stickyEnd = False
    commitNames = []

    for node in orderedList:
        commitNames.insert(len(commitNames), node.commit_hash)
    
    for commit in range(0, len(orderedList)):
        if (stickyEnd == True):
            print('=')
            string = orderedList[commit].commit_hash
            for i in orderedList[commit].branch:
                string = string + ' ' + i
            print(string)
            stickyEnd = False
        else:
            
            if (commit + 1 == len(orderedList)):
                string = orderedList[commit].commit_hash
                for i in orderedList[commit].branch:
                    string = string + ' ' + i
                print(string)
                continue
            else:
                found = False
                for i in orderedList[commit].parents:
                    if i == orderedList[commit + 1].commit_hash:
                        found = True
                        break
                if (found == True):
                    string = orderedList[commit].commit_hash
                    for i in orderedList[commit].branch:
                        string = string + ' ' + i
                    print(string)
                else:
                    string = orderedList[commit].commit_hash
                    for i in orderedList[commit].branch:
                        string = string + ' ' + i
                    print(string)
                    
                    count = 0
                    for i in orderedList[commit].parents:
                        count = count + 1
                        if (count == len(orderedList[commit].parents)):
                            print(i + '=\n')
                        else:
                            print(i)

                    stickyEnd = True
        
    visited = [False]*len(commitNames)
    roots = 0
    counter = 0

    for node in orderedList:
        if (len(node.children) == 0):
            roots = roots + 1
    
    toBeReturned = ""

    #for index in range(0, len(orderedList)):
        #if (len(orderedList[index].children) == 0):
            #counter = counter + 1
            #if (counter != roots):
                #toBeReturned = printing(orderedList[index], index, orderedList, commitNames, visited, False)
            #else:
                #toBeReturned =  printing(orderedList[index], index, orderedList, commitNames, visited, True)
            
                 
    
if __name__ == '__main__':
    topo_order_commits()
