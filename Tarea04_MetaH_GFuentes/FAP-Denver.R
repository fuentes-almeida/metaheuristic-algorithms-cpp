#PONER LA DIRECCION DE LA CARPETA DONDE SE ENCUENTRAN LOS RESULTADOS QUE GENERO EL PROGRAMA
setwd("/home/juan-j2c/Dropbox/Metaheuristicas/Tarea04_MetaH_GFuentes/ResultAnalysis/Data")

#se lee cada archivo y se guarda en un data.frame
CDRW<-read.table("CDRW.txt",header = TRUE)
CLR_1<-read.table("CLR_1.txt",header = TRUE)
CLR_2<-read.table("CLR_2.txt",header = TRUE)
CLR_5<-read.table("CLR_5.txt",header = TRUE)
A5<-read.table("A5.txt",header = TRUE)
A5_scaled<-read.table("A5_scaled.txt",header = TRUE)
RTS_2<-read.table("RTS_2.txt",header = TRUE)
RTS_5<-read.table("RTS_5.txt",header = TRUE)
RTS_10<-read.table("RTS_10.txt",header = TRUE)
RTS_25<-read.table("RTS_25.txt",header = TRUE)
RTS_50<-read.table("RTS_50.txt",header = TRUE)
AGCR_025<-read.table("AGCR_025.txt",header = TRUE)
AGCR_075<-read.table("AGCR_075.txt",header = TRUE)
GEN_ELIT<-read.table("GEN_ELIT.txt",header = TRUE)
COMB<-read.table("COMB.txt",header = TRUE)
RW<-read.table("RW.txt",header = TRUE)
MULTY_DYNAMIC<-read.table("MULTY_DYNAMIC.txt",header = TRUE)

#El archivo de resultados se traspone para que los nombres queden en el 1er renglon
Results<-as.data.frame(t(read.table("Results.txt",row.names = 1)))
#se ondenan alfabeticamente
Results<-Results[c("CDRW","CLR_1","CLR_2","CLR_5","A5","A5_scaled","RTS_2","RTS_5","RTS_10"
                   ,"RTS_25","RTS_50","AGCR_025","AGCR_075","GEN_ELIT","COMB","RW","MULTY_DYNAMIC")]
names=colnames(Results)

#Se genera una tabla para calcular las medidas estadisticas del fitness de cada algoritmo
table<-matrix(rep(0,17*5),ncol=5,byrow  = 17)
colnames(table)=c("Mean","Median","StdDev","Max","Min")
rownames(table)=names
for (i in 1:17)
{
  table[i,1]=mean(Results[,i])
  table[i,2]=median(Results[,i])
  table[i,3]=sd(Results[,i])
  table[i,4]=max(Results[,i])
  table[i,5]=min(Results[,i])
}
table

#Se genera un vector de medias y medianas para cada algoritmo a partir de los resultados
Res_Means<-rep(0,17)
Res_Medians<-rep(0,17)
for (i in 1:17)
{
  Res_Means[i]=mean(Results[,i])
  Res_Medians[i]=median(Results[,i])
}

#se genera un vector con los resultados del test de normalidad
ShapiroWilk<-rep(0,17)
for (i in 1:17)
{
  test=shapiro.test(Results[,i])
  ShapiroWilk[i]=test$p.value
}

#se identifica a que algoritmo pertenece cada valor
names(ShapiroWilk)=names

#si pvalue>0.1 la muestra proviene de una dist normal
ShapiroWilk

#libreria para pruebas estadisticas
library(car)
group<-as.factor(c(rep(1,30),rep(2,30)))

#se crea una matriz para guardar los resultados de los torneos
tournament=matrix(rep(0,17*17),ncol=17,byrow  = 17)
colnames(tournament)=names
rownames(tournament)=names

for (i in 1:17)
{
  for (j in 1:17)
  {
    #se calculan medias y medianas de los datos que vamos a analizar
    mean1=Res_Means[i]
    mean2=Res_Means[j]
    median1=Res_Medians[i]
    median2=Res_Medians[j]
    
    #se verifica normalidad para dos algoritmos seleccionados
    if (ShapiroWilk[i]>0.05 && ShapiroWilk[j]>0.05)
    {
      #se verifica homogeneidad de varianzas  
      test=leveneTest(c(Results[,i],Results[,j]),group)
      if (test$`Pr(>F)`[1]>0.05)
      {
        #si hay normalidad y varianzas iguales, se hace ANOVA
        test=summary(aov(c(Results[,i],Results[,j])~group))
        #si pvalue<0.05 existe una diferencia estadisticamente significativa entre las medias
        if (test[[1]][[5]][[1]]<0.05)
        {
          if (mean1<mean2)
          {
            tournament[j,i]=1
            tournament[i,j]=-1
          }
          
          if (mean1>mean2)
          {
            tournament[i,j]=1
            tournament[j,i]=-1
          }
        }
      }
      #si no hay igualdad de varianzas, se hace prueba Welch
      else
      {
        test=t.test(Results[,i],Results[,j],alternative = "two.sided",var.equal = FALSE)
        #si pvalue<0.05 existe diferencia significativa entre las medias
        if (test$p.value<0.05)
        {
          if (mean1<mean2)
          {
            tournament[j,i]=1
            tournament[i,j]=-1
          }
          
          if (mean1>mean2)
          {
            tournament[i,j]=1
            tournament[j,i]=-1
          }
        }
      }
    }
    #si no hay normalidad entre los algoritmos se hace la prueba de kruskal wallis
    else
    {
      test=kruskal.test(c(Results[,i],Results[,j]),group)
      #si pvalue<0.05 existe diferencia significativa entre las medianas
      if (test$p.value<0.05)
      {
        if (median1<median2)
        {
          tournament[j,i]=1
          tournament[i,j]=-1
        }
          
        if (median1>median2)
        {
          tournament[i,j]=1
          tournament[j,i]=-1
        }
      }
    }
  }
}

#cada 1 en la columna de la matriz significa que el algoritmo gana contra el algoritmo el respectivo renglon
#un -1 significa que el algoritmo perdio
tournament

#se calcula el score de cada algoritmo
score<-matrix(rep(0,17*3),ncol=17,byrow  = 3)
for (i in 1:17)
{
  for (j in 1:17)
  {
    if (tournament[j,i]==1)
      score[1,i]=score[1,i]+1
    if (tournament[j,i]==-1)
      score[2,i]=score[2,i]+1
  }
  score[3,i]=sum(tournament[,i])
}
colnames(score)=names
rownames(score)=c("win","lost","total")
score

#libreria para dibujar las graficas
library(ggplot2)
library(reshape2)
library(cowplot)

#Boxplot con los Resultados de cada algoritmo
boxplot(Results,las=2,main="Average Interference by Algorithm (mean is the black dot)",col=rainbow(17),par(cex.axis=0.7))
points(Res_Means, col="black", pch=18)

#Grafica de las Medias obtenidas para cada algoritmo
Means1<-data.frame(CDRW$Mean,CLR_1$Mean,CLR_2$Mean,CLR_5$Mean,time=seq(1/6,48,1/6))
Means2<-data.frame(A5$Mean,A5_scaled$Mean,AGCR_025$Mean,AGCR_075$Mean,time=seq(1/6,48,1/6))
Means3<-data.frame(RTS_2$Mean,RTS_5$Mean,RTS_10$Mean,RTS_25$Mean,RTS_50$Mean,time=seq(1/6,48,1/6))
Means4<-data.frame(GEN_ELIT$Mean,COMB$Mean, RW$Mean,MULTY_DYNAMIC$Mean,time=seq(1/6,48,1/6))

Means1 <- melt(Means1, id="time")  # convert to long format
Means2 <- melt(Means2, id="time")  # convert to long format
Means3 <- melt(Means3, id="time")  # convert to long format
Means4 <- melt(Means4, id="time")  # convert to long format

p1<-ggplot(data=Means1,aes(x=time, y=value, colour=variable)) + geom_line()
p2<-ggplot(data=Means2,aes(x=time, y=value, colour=variable)) + geom_line()
p3<-ggplot(data=Means3,aes(x=time, y=value, colour=variable)) + geom_line()
p4<-ggplot(data=Means4,aes(x=time, y=value, colour=variable)) + geom_line()
plot_grid(p1, p2, p3, p4,  ncol = 2, nrow = 2)


#Grafica de las Medianas obtenidas para cada algoritmo
Medians1<-data.frame(CDRW$Median,CLR_1$Median,CLR_2$Median,CLR_5$Median,time=seq(1/6,48,1/6))
Medians2<-data.frame(A5$Median,A5_scaled$Median,AGCR_025$Median,AGCR_075$Median,time=seq(1/6,48,1/6))
Medians3<-data.frame(RTS_2$Median,RTS_5$Median,RTS_10$Median,RTS_25$Median,RTS_50$Median,time=seq(1/6,48,1/6))
Medians4<-data.frame(GEN_ELIT$Median,COMB$Median, RW$Median,MULTY_DYNAMIC$Median,time=seq(1/6,48,1/6))

Medians1 <- melt(Medians1, id="time")  # convert to long format
Medians2 <- melt(Medians2, id="time")  # convert to long format
Medians3 <- melt(Medians3, id="time")  # convert to long format
Medians4 <- melt(Medians4, id="time")  # convert to long format

p1<-ggplot(data=Medians1,aes(x=time, y=value, colour=variable)) + geom_line()
p2<-ggplot(data=Medians2,aes(x=time, y=value, colour=variable)) + geom_line()
p3<-ggplot(data=Medians3,aes(x=time, y=value, colour=variable)) + geom_line()
p4<-ggplot(data=Medians4,aes(x=time, y=value, colour=variable)) + geom_line()
plot_grid(p1, p2, p3, p4,  ncol = 2, nrow = 2)


#Grafica de las Desviaciones Estandar obtenidas para cada algoritmo
StdDevs1<-data.frame(CDRW$StdDev,CLR_1$StdDev,CLR_2$StdDev,CLR_5$StdDev,time=seq(1/6,48,1/6))
StdDevs2<-data.frame(A5$StdDev,A5_scaled$StdDev,AGCR_025$StdDev,AGCR_075$StdDev,time=seq(1/6,48,1/6))
StdDevs3<-data.frame(RTS_2$StdDev,RTS_5$StdDev,RTS_10$StdDev,RTS_25$StdDev,RTS_50$StdDev,time=seq(1/6,48,1/6))
StdDevs4<-data.frame(GEN_ELIT$StdDev,COMB$StdDev, RW$StdDev,MULTY_DYNAMIC$StdDev,time=seq(1/6,48,1/6))

StdDevs1 <- melt(StdDevs1, id="time")  # convert to long format
StdDevs2 <- melt(StdDevs2, id="time")  # convert to long format
StdDevs3 <- melt(StdDevs3, id="time")  # convert to long format
StdDevs4 <- melt(StdDevs4, id="time")  # convert to long format

p1<-ggplot(data=StdDevs1,aes(x=time, y=value, colour=variable)) + geom_line()
p2<-ggplot(data=StdDevs2,aes(x=time, y=value, colour=variable)) + geom_line()
p3<-ggplot(data=StdDevs3,aes(x=time, y=value, colour=variable)) + geom_line()
p4<-ggplot(data=StdDevs4,aes(x=time, y=value, colour=variable)) + geom_line()
plot_grid(p1, p2, p3, p4,  ncol = 2, nrow = 2)

#Grafica de los valores maximos obtenidos para cada algoritmo
Maxs1<-data.frame(CDRW$Max,CLR_1$Max,CLR_2$Max,CLR_5$Max,time=seq(1/6,48,1/6))
Maxs2<-data.frame(A5$Max,A5_scaled$Max,AGCR_025$Max,AGCR_075$Max,time=seq(1/6,48,1/6))
Maxs3<-data.frame(RTS_2$Max,RTS_5$Max,RTS_10$Max,RTS_25$Max,RTS_50$Max,time=seq(1/6,48,1/6))
Maxs4<-data.frame(GEN_ELIT$Max,COMB$Max, RW$Max,MULTY_DYNAMIC$Max,time=seq(1/6,48,1/6))

Maxs1 <- melt(Maxs1, id="time")  # convert to long format
Maxs2 <- melt(Maxs2, id="time")  # convert to long format
Maxs3 <- melt(Maxs3, id="time")  # convert to long format
Maxs4 <- melt(Maxs4, id="time")  # convert to long format

p1<-ggplot(data=Maxs1,aes(x=time, y=value, colour=variable)) + geom_line()
p2<-ggplot(data=Maxs2,aes(x=time, y=value, colour=variable)) + geom_line()
p3<-ggplot(data=Maxs3,aes(x=time, y=value, colour=variable)) + geom_line()
p4<-ggplot(data=Maxs4,aes(x=time, y=value, colour=variable)) + geom_line()
plot_grid(p1, p2, p3, p4,  ncol = 2, nrow = 2)

#Grafica de los valores minimos obtenidos para cada algoritmo
Mins1<-data.frame(CDRW$Min,CLR_1$Min,CLR_2$Min,CLR_5$Min,time=seq(1/6,48,1/6))
Mins2<-data.frame(A5$Min,A5_scaled$Min,AGCR_025$Min,AGCR_075$Min,time=seq(1/6,48,1/6))
Mins3<-data.frame(RTS_2$Min,RTS_5$Min,RTS_10$Min,RTS_25$Min,RTS_50$Min,time=seq(1/6,48,1/6))
Mins4<-data.frame(GEN_ELIT$Min,COMB$Min, RW$Min,MULTY_DYNAMIC$Min,time=seq(1/6,48,1/6))

Mins1 <- melt(Mins1, id="time")  # convert to long format
Mins2 <- melt(Mins2, id="time")  # convert to long format
Mins3 <- melt(Mins3, id="time")  # convert to long format
Mins4 <- melt(Mins4, id="time")  # convert to long format

p1<-ggplot(data=Mins1,aes(x=time, y=value, colour=variable)) + geom_line()
p2<-ggplot(data=Mins2,aes(x=time, y=value, colour=variable)) + geom_line()
p3<-ggplot(data=Mins3,aes(x=time, y=value, colour=variable)) + geom_line()
p4<-ggplot(data=Mins4,aes(x=time, y=value, colour=variable)) + geom_line()
plot_grid(p1, p2, p3, p4,  ncol = 2, nrow = 2)

#Grafica de los valores de Entropia obtenidos para cada algoritmo
Entropys1<-data.frame(CDRW$Entropy,CLR_1$Entropy,CLR_2$Entropy,CLR_5$Entropy,time=seq(1/6,48,1/6))
Entropys2<-data.frame(A5$Entropy,A5_scaled$Entropy,AGCR_025$Entropy,AGCR_075$Entropy,time=seq(1/6,48,1/6))
Entropys3<-data.frame(RTS_2$Entropy,RTS_5$Entropy,RTS_10$Entropy,RTS_25$Entropy,RTS_50$Entropy,time=seq(1/6,48,1/6))
Entropys4<-data.frame(GEN_ELIT$Entropy,COMB$Entropy, RW$Entropy,MULTY_DYNAMIC$Entropy,time=seq(1/6,48,1/6))

Entropys1 <- melt(Entropys1, id="time")  # convert to long format
Entropys2 <- melt(Entropys2, id="time")  # convert to long format
Entropys3 <- melt(Entropys3, id="time")  # convert to long format
Entropys4 <- melt(Entropys4, id="time")  # convert to long format

p1<-ggplot(data=Entropys1,aes(x=time, y=value, colour=variable)) + geom_line()
p2<-ggplot(data=Entropys2,aes(x=time, y=value, colour=variable)) + geom_line()
p3<-ggplot(data=Entropys3,aes(x=time, y=value, colour=variable)) + geom_line()
p4<-ggplot(data=Entropys4,aes(x=time, y=value, colour=variable)) + geom_line()
plot_grid(p1, p2, p3, p4,  ncol = 2, nrow = 2)

#Grafica de los valores de distancia Hamming obtenidos para cada algoritmo
Hammings1<-data.frame(CDRW$Hamming,CLR_1$Hamming,CLR_2$Hamming,CLR_5$Hamming,time=seq(1/6,48,1/6))
Hammings2<-data.frame(A5$Hamming,A5_scaled$Hamming,AGCR_025$Hamming,AGCR_075$Hamming,time=seq(1/6,48,1/6))
Hammings3<-data.frame(RTS_2$Hamming,RTS_5$Hamming,RTS_10$Hamming,RTS_25$Hamming,RTS_50$Hamming,time=seq(1/6,48,1/6))
Hammings4<-data.frame(GEN_ELIT$Hamming,COMB$Hamming, RW$Hamming,MULTY_DYNAMIC$Hamming,time=seq(1/6,48,1/6))

Hammings1 <- melt(Hammings1, id="time")  # convert to long format
Hammings2 <- melt(Hammings2, id="time")  # convert to long format
Hammings3 <- melt(Hammings3, id="time")  # convert to long format
Hammings4 <- melt(Hammings4, id="time")  # convert to long format

p1<-ggplot(data=Hammings1,aes(x=time, y=value, colour=variable)) + geom_line()
p2<-ggplot(data=Hammings2,aes(x=time, y=value, colour=variable)) + geom_line()
p3<-ggplot(data=Hammings3,aes(x=time, y=value, colour=variable)) + geom_line()
p4<-ggplot(data=Hammings4,aes(x=time, y=value, colour=variable)) + geom_line()
plot_grid(p1, p2, p3, p4,  ncol = 2, nrow = 2)

#Grafica de los valores de Run Length Distribution obtenidos para cada algoritmo
RLDs1<-data.frame(CDRW$RLD,CLR_1$RLD,CLR_2$RLD,CLR_5$RLD,time=seq(1/6,48,1/6))
RLDs2<-data.frame(A5$RLD,A5_scaled$RLD,AGCR_025$RLD,AGCR_075$RLD,time=seq(1/6,48,1/6))
RLDs3<-data.frame(RTS_2$RLD,RTS_5$RLD,RTS_10$RLD,RTS_25$RLD,RTS_50$RLD,time=seq(1/6,48,1/6))
RLDs4<-data.frame(GEN_ELIT$RLD,COMB$RLD, RW$RLD,MULTY_DYNAMIC$RLD,time=seq(1/6,48,1/6))

RLDs1 <- melt(RLDs1, id="time")  # convert to long format
RLDs2 <- melt(RLDs2, id="time")  # convert to long format
RLDs3 <- melt(RLDs3, id="time")  # convert to long format
RLDs4 <- melt(RLDs4, id="time")  # convert to long format

p1<-ggplot(data=RLDs1,aes(x=time, y=value, colour=variable)) + geom_line()
p2<-ggplot(data=RLDs2,aes(x=time, y=value, colour=variable)) + geom_line()
p3<-ggplot(data=RLDs3,aes(x=time, y=value, colour=variable)) + geom_line()
p4<-ggplot(data=RLDs4,aes(x=time, y=value, colour=variable)) + geom_line()
plot_grid(p1, p2, p3, p4,  ncol = 2, nrow = 2)



















