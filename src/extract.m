%Creates a graph from a statistic file and prints it into a pdf
function []= extract(fname)
fid=fopen(fname,'r')
%Read parameters from file
msize = fscanf(fid, '%i',1)
nrmul = fscanf(fid, '%i',1)
popsize = fscanf(fid, '%i',1)
mutratio = fscanf(fid, '%f',1)
crossratio = fscanf(fid, '%f',1)
replratio = fscanf(fid, '%f',1)
nrgen = fscanf(fid, '%i',1)
bestscore = fscanf(fid, '%f',1)

N=10;
Gens=zeros(1,N);
Avg=zeros(1,N);
Best=zeros(1,N);
Worst=zeros(1,N);
Variance=zeros(1,N);
Minus=0;
AvgplusVar=zeros(1,N);
AvgminusVar=zeros(1,N);

hold all
grid on
%Loop and read all the values from the file
for i=1:N+1
   Gens(1,i)=fscanf(fid,'%f',1);
   Avg(1,i)=fscanf(fid,'%f',1);
   Best(1,i)=fscanf(fid,'%f',1);
   Worst(1,i)=fscanf(fid,'%f',1);
   Variance(1,i)=fscanf(fid,'%f',1);
   Minus=fscanf(fid,'%f',1);
   AvgplusVar(1,i)=Avg(1,i)+Variance(1,i);
   AvgminusVar(1,i)=Avg(1,i)-Variance(1,i);

end


%Generating the graph
plot(Gens(1,:),Best(1,:),'b','LineWidth',1);
plot(Gens(1,:),Worst(1,:),'r','LineWidth',1);
plot(Gens(1,:),Avg(1,:),'g','LineWidth',2);
%plot(Gens(1,:),AvgplusVar(1,:),'k--');
%plot(Gens(1,:),AvgminusVar(1,:),'k--');
errorbar(Gens(1,:),Avg(1,:),Variance(1,:));

text(Gens(1,N/2),Best(1,N/2),'Maxfitness \rightarrow',...
     'HorizontalAlignment','right')

text(Gens(1,N/2),Avg(1,N/2),'Avgfitness \rightarrow',...
     'HorizontalAlignment','right')
 
text(Gens(1,N/2),Worst(1,N/2),'\leftarrow Minfitness',...
     'HorizontalAlignment','left')
 
xlabel('Generations');
ylabel('Fitness');


nrmulstr = strcat('nrmul',int2str(nrmul));
msizestr = strcat('msize',int2str(msize));
popsizestr = strcat('pop',int2str(popsize));
mutratiostr = strcat('mut',num2str(mutratio));
crossratiostr = strcat('cross',num2str(crossratio));
replratiostr = strcat('repl',num2str(replratio));
nrgenstr = strcat('gen',int2str(nrgen));
bestscorestr = strcat('best',num2str(bestscore));


uistr(1) = {msizestr};
uistr(2) = {nrmulstr};
uistr(3) = {popsizestr};
uistr(4) = {mutratiostr};
uistr(5) = {crossratiostr};
uistr(6) = {replratiostr};
uistr(7) = {nrgenstr};

uicontrol('Style','text','Position',[100 390 130 110],...
          'String',uistr);
h=gcf;
fname=strcat(msizestr,'_',nrmulstr,'_',popsizestr,'_',mutratiostr,'_',crossratiostr,'_',replratiostr,'_',nrgenstr,'_',bestscorestr,'.pdf');
print(h,fname);
