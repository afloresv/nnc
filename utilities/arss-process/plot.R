library(ggplot2)
library(ggforce)

r = read.table(file="data.txt", header=FALSE, skip=2, col.names=c("fcv","x","y","cls"))
r$cls = factor(r$cls)

sel = read.csv(file="sel05.csv")
sel$ind = 1 + sel$ind

format_plot <- function(p) {
  p <- p + theme_void() +
    coord_fixed(xlim=c(-1,2.5), ylim=c(-0.5,2), expand=FALSE) +
    scale_colour_manual(values = c("0"="#F8766D", "1"="#00BFC4", "2"="#888888", "3"="#BBBBBB")) +
    scale_linetype_manual(values = c("1"="solid", "2"="dotted")) +
    theme(legend.position="none")
  return(p)
}

plotSelAMSS <- function(num) {
  
  fname = paste("img/sel-", num, ".pdf", sep="")
  if (num==85) {
    sel = data.frame(ind=c(1+813, sel$ind), dne=c(0.374269, sel$dne))
    num = 82
  }
  
  rs = data.frame(
    x = r$x[sel$ind[1:num]],
    y = r$y[sel$ind[1:num]],
    cls = r$cls[sel$ind[1:num]],
    dne = sel$dne[1:num],
    rad = sel$dne[1:num] / 1.5
  )
  
  p <- ggplot(rs) +
    #Points & Balls
    geom_point(size=1, aes(x=x, y=y, colour=cls), data=r, alpha=0.1) +
    geom_circle(aes(x0=x, y0=y, r=rad, linetype="1", color="2")) +
    geom_circle(aes(x0=x, y0=y, r=dne, linetype="2", color="3")) +
    geom_point(size=1, aes(x=x, y=y, colour=cls))
  p <- format_plot(p)
  
  ggsave(fname, device="pdf", width=28, height=20, units="cm")
}

p <- format_plot(ggplot(r) + geom_point(size=1, aes(x=x, y=y, colour=cls)))
ggsave("img/data.pdf", device="pdf", width=28, height=20, units="cm")

for (lim in 77:85) {
  plotSelAMSS(lim)
}