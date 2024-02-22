function [clustCent,data2cluster, clustLocate] = MeanShiftCluster2(dataPts, locate)

 
%%
[numDim,numPts] = size(dataPts);
numClust        = 0;
initPtInds      = 1:numPts;
stopThresh      = 20;         
dataPts_n       = dataPts';
thre1           = mean(std(dataPts_n));
same_Cluster    = thre1;
dis_attri   = 0.5;
color_attri = 1-dis_attri;

clustCent       = [];                                   
beenVisitedFlag = zeros(1,numPts);                 
numInitPts      = numPts;                        
clusterVotes    = zeros(1,numPts);                

alpha = 255/sqrt(numPts);

 %%
while (numClust<1000&&numInitPts~=0)
    tempInd         = ceil(numInitPts/2);
    stInd           = initPtInds(tempInd);     
    myMean          = dataPts(:,stInd);             
    stLocate        = locate(:,stInd);
    myMembers       = [];                                                     
    thisClusterVotes    = zeros(1,numPts);            
    iter = 0;
    
    while 1     %loop untill convergence
        iter = iter +1;
        sqDistToAll = repmat(myMean,1,numPts) - dataPts;   
        norm_color = zeros([1,numPts]);
        for i = 1:numPts
            norm_color(i) = norm(sqDistToAll(:,i));
        end
        locateToAll = repmat(stLocate,1,numPts) - locate; 
        norm_locate = zeros([1,numPts]);
        for i = 1:numPts
            norm_locate(i) = norm(locateToAll(:,i))*alpha;
        end
        Toatal_distance = norm_color*color_attri + norm_locate*dis_attri;

        inInds      = find(Toatal_distance < thre1);                  
        thisClusterVotes(inInds) = thisClusterVotes(inInds)+1;      
        PtsinClust  = length(inInds);
         
        myOldMean   = myMean;                                   
        myMean      = mean(dataPts(:,inInds),2);           
        myOldLocate = stLocate;
        stLocate    = mean(locate(:,inInds),2);
        
        myMembers   = [myMembers inInds];                    
        beenVisitedFlag(myMembers) = 1;                     
         
        
        if (norm((myMean-myOldMean))*color_attri + norm(stLocate-myOldLocate)*alpha*dis_attri)< stopThresh
             
            %check for merge posibilities
            mergeWith = 0;
            for cN = 1:numClust
                distToOther = norm(myMean-clustCent(:,cN))*color_attri+(norm(stLocate-clustLocate(:,cN)))*alpha*dis_attri;     
                if distToOther < same_Cluster                  
                    mergeWith = cN;
                    break;
                end
            end
             
             
            if mergeWith > 0  
                clustCent(:,mergeWith)       = (myMean*PtsinClust+clustCent(:,mergeWith)*ClustPts)/(PtsinClust+ClustPts);           
                clustLocate(:,mergeWith)      = (stLocate*PtsinClust+clustLocate(:,mergeWith)*ClustPts)/(PtsinClust+ClustPts);
                clusterVotes(mergeWith,:)    = clusterVotes(mergeWith,:) + thisClusterVotes;    
            else  
                numClust                    = numClust+1;                
                clustCent(:,numClust)       = myMean;                
                clustLocate(:,numClust)     = stLocate;
                clusterVotes(numClust,:)    = thisClusterVotes;
                ClustPts(numClust)        = PtsinClust;
            end
 
            break;
        end
 
    end

    initPtInds      = find(beenVisitedFlag == 0);    
    numInitPts      = length(initPtInds);           
end
 %%
[val,data2cluster] = max(clusterVotes,[],1);      
 