#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <windows.h>
char ndf[4]="ndf",//not defined yet
rw_[4]="rw_", //droit par default personel
r__[4]="r__", //droit par default public
FFF[4]="FFF"; //code error

struct repertoires {    // POUR LA MANIPULATION DES REPERTOIRES
    char             nom[20];
    struct repertoires * suiv;
    struct fichiers    * fich;  // les s-fichier
};
struct fichiers {        // POUR LA MANIPULATION DES FICHIERS
    struct fichiers * suiv;
    char          nom[20];
    char          createur[20];  //utilisateur qui a cree ce fichier
};
struct groupes {       // POUR LA MANIPULATION GROUPES 
    char              nom[20];
    struct groupes    * suiv;
    struct users    * users;   /* la list des utilisateur qui appartient a ce groupe*/
    struct pr_fichiers * rights;  //les droits des fichiers
    int                 perso; //discription si desus
};
/* pourquoi le champ perso?: pour destinger les groupe personnels est les groupe crier par l admin
    -si le champ perso est a 1 le groupe est personnel:
        ie: 1-il ne peut contenir que un seul urilisateur
            2-il porte le nom de cette utilisateur    blablabal 
            3-il port les droit perso de cette utilisateur
            4-il est crier automatiquement
    -si il est a 0: il est un groupe public
        ie: 1-il est crier par l'admin
            2-il contint les droit par default pour tout les utilisareur
*/



struct pr_groupes{    
    struct pr_groupes * suiv;
    char nom[20];
};
struct users {                  /*le maillon des utilisateurs*/
    struct users * suiv;
    int            ID;          /*l'identificateur*/
    char       nom[20];
    struct pr_groupes * pr_groupe;  // les groupes de ce utilisateur
};
struct pr_fichiers{  /*le maillon  qui represente chaque fichier dans le groupe (par son nom et les droits atribue aux membres du groupe par aport a ce fichier*/
    struct pr_fichier * suiv;
    char            nom[20];
    char            droit[20];
};
typedef struct pr_fichiers * pr_fichier;
typedef struct users * user;
typedef struct groupes * groupe;
typedef struct fichiers * fichier;
typedef struct repertoires * repertoire;
typedef struct pr_groupes * pr_groupe;


/*les fonctions pour la manipulation des groupes et des utilisateurs  */
void add_groupe(groupe *nouveau);                         //CES FONCTIONS 
void g_aff_suiv(groupe p,groupe q);
void u_aff_suiv(user u,user q);                                // POUR CIEER USERS ET GROUPES 
void g_aff_droit(groupe g,char fich[20],char d[20]);
void g_add_user(groupe g,user u);
void add_user(user *u);
void creat_user(user u,char nom [20]);                       /*creer un user*/
int trouve_groupe( groupe racine,char nom[20],groupe *adr); /*elle retourne un pointeur vers un groupe */
void sup_user(user *u_tete,char user[],groupe g_tete);     /*suprimer un compt donnee*/
void afficher_user(user u_tete)  ;                         /*afficher tous les utilisateurs qui sont creer par l'administrateur evec les groupes qui sont l'appartient*/
void sup_user(user *u_tete,char user_nom[20],groupe g_tete); /*suprimer un utilisateur*/


/* les fonction pour la manipulation des fichier */
void add_fich(repertoire rep_tete,char repnom[],char fichnom[],char creator[]);/*ajouter un fichier */
void affich_fich(repertoire rep_tete,char rep_nom[])  ;          /* afficher tous les sous ficher d'un repertoire */


/* les fonction pour la manipulation des repertoire*/
void add_rep(repertoire *rep_tete,char nom[20]);   /*crieer un nouveau sous-repertoire*/
void affich_rep(repertoire rep_tete);              /*afficher tous les repertoires qui existes */

void u_aff_groupe(user u,groupe g){
pr_groupe p,pp;
if (u->pr_groupe ==NULL){ //l'utilisateur n'est pas encor dans le groupe perso
p = u->pr_groupe;
pp=(pr_groupe)malloc(sizeof(struct pr_groupes));
strcpy(pp->nom,g->nom);
pp->suiv=p;
u->pr_groupe=pp;
}
else{ // l'utilisateur est dans le groupe perso qui doit rester le premier dans la list des groupes de l'utilisateur
p = u->pr_groupe->suiv;
pp=(pr_groupe)malloc(sizeof(struct pr_groupes));
strcpy(pp->nom,g->nom);
pp->suiv=p;
u->pr_groupe->suiv=pp;
}
}
void sup_user(user *u_tete,char user_nom[20],groupe g_tete){  // spprimer un user
    user u,uu,y,yy;
    groupe gg;
 if (*u_tete!=NULL)
 {
                u=*u_tete;
                if (strcmp(u->nom,user_nom)==0)
                {
                    y=u;
                    *u_tete=u->suiv;
                    free(y);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x03);
                    printf("\n==> l'utilisateur %s a ete bien suprimer ",user_nom);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
                }
                else
                {while (u->suiv!=NULL && strcmp(u->suiv->nom,user_nom)!=0)
                {
                    u=u->suiv;
                }
                if (u->suiv!=NULL && strcmp(u->suiv->nom,user_nom)==0)/* on ne peut pas suprimer le root donc on ne firifie pas le premier maillon*/
                {
                    y=u->suiv;
                    u->suiv=y->suiv;
                    free(y);

                    if (g_tete->users!=NULL && strcmp(g_tete->users->nom,user_nom)==0)
                        {
                           y=g_tete->users;
                           g_tete->users=g_tete->users->suiv;
                           free(y);
                        }
                    gg=g_tete;
                    while(gg!=NULL)
                    {
                       uu=gg->users;

                            while (uu!=NULL && uu->suiv!=NULL && strcmp(uu->suiv->nom,user_nom)!=0)
                            {
                                uu=uu->suiv;

                            }
                            if (uu!=NULL && uu->suiv!=NULL && strcmp(uu->suiv->nom,user_nom)==0)
                            {
                                yy=uu->suiv ;
                                uu->suiv=yy->suiv ;
                                free(yy);
                            }

                     gg=gg->suiv;
                     }
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
                    printf("\n==> l'utilisateur %s a ete bien suprimer ",user_nom);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
                }
                else
                {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
                    printf("\n!!! dezole mais l'utilisateur %s n'xiste pas ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
                }
                }
 }
 else
 {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C); // texte rouge (C) sur fond jaune (E)
    printf("\n!!! il n'existe aucun utilisateur :");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F); // texte rouge (C) sur fond jaune (E)
}
 }
void affiche_groupe(groupe g_tete){
    groupe g;
    int cont=1;
    user u;
    if (g_tete!=NULL)
    {
       g=g_tete;
       while (g!=NULL)
       {
           printf("\n Groupe %d est: '%s' et il contient ",cont,g->nom);
           cont++;
           if (g->users!=NULL)
           {
               u=g->users;
               while (u!=NULL)
               {
                   printf("user: '%s' , ",u->nom);
                   u=u->suiv;
               }
           }
           else
           {
               SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
               printf("aucun user!");
               SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
           }
           g=g->suiv;
       }
    }
    else
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
        printf("\n!!  il n'existe aucun groupe ");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
    }
}
void afficher_user(user u_tete){
    user u;
    pr_groupe g;
    int cont=1;
    if (u_tete!=NULL)
    {
        u=u_tete;
        while (u!=NULL)
        {
            printf("\n _user %d est: '%s'    il apartien a :",cont,u->nom);
            cont++;
            if (u->pr_groupe!=NULL)
            {
              g=u->pr_groupe ;
              while(g!=NULL)
              {
                  printf("groupe: '%s' ;",g->nom);
                  g=g->suiv;
              }
            }
            else
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
                printf("  aucun groupe ");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
            }
            u=u->suiv;
        }
    }
    else
    {
         SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
        printf("\n!!  il n'xiste aucun utilisateur ;");
         SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
    }
}
void u_aff_suiv(user u,user q){
u->suiv = q;
}
void add_groupe(groupe *nouveau){
*nouveau = (groupe)malloc(sizeof(struct groupes));
}
void g_aff_suiv(groupe p,groupe q){
p->suiv = q;
}
void g_add_fichier(groupe g,char fichier[20],char d[20]){
pr_fichier p;
p = g->rights;
g->rights = (pr_fichier)malloc(sizeof(struct pr_fichiers));
strcpy(g->rights->droit,d);
strcpy(g->rights->nom,fichier);
g->rights->suiv = p;
}
void g_add_user(groupe g,user u){
user p,q;
p = g->users;
q=(user)malloc(sizeof (struct  users));
q->suiv=p;
strcpy(q->nom,u->nom);
g->users=q;
u_aff_groupe(u,g);

}
void add_user(user *u){
*u = (user)malloc(sizeof(struct users));
}
int trouve_groupe( groupe racine,char nom[20],groupe *adr){ // retourne un pointeur vers un groupr donnee
groupe g;
g=racine;
while ((g!=NULL)&&(strcmp(g->nom,nom)!=0)){
    g = g->suiv;
}
if (g==NULL){
    return 0;
}
else {
    *adr= g;
    return 1;
}
}
int gene_id(){        // donner un id au user
int ID;
ID = rand() %100000;
return ID;
}
void creat_user(user u,char nom [20]){         // creer un user
int a;
a= gene_id();
u->ID = a;
strcpy(u->nom,nom);
u->suiv = NULL;
u->pr_groupe = NULL;
}
void crier_groupe(groupe *g_tete,char nom[20]){
groupe g,gg;
g=*g_tete;
gg= (groupe)malloc(sizeof (struct groupes));
strcpy(gg->nom,nom);
gg->rights = NULL;
gg->suiv = g;
gg->users = NULL;
gg->perso =0;
*g_tete=gg;
}
void rech_compt(user u_tete,char u_nom[]){        //recherche d'un compte
    user u;
    pr_groupe g;
    int cont=1;
    if (u_tete!=NULL)
    {
        u=u_tete;
        while(u!=NULL && strcmp(u->nom,u_nom)!=0)
        {
            u=u->suiv;
        }
        if (u!=NULL && strcmp(u->nom,u_nom)==0)
        {
            printf("l'utilisateur '%s' existe et il apartient a :",u_nom);
            if (u->pr_groupe!=NULL)
            {
              g=u->pr_groupe ;
              while(g!=NULL)
              {
                  printf("groupe: %s ;",g->nom);
                  g=g->suiv;
              }
            }
            else
            {
                printf("  aucun groupe ");
            }

        }
        else
        {
         SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
         printf("\n lutilisateur '%s' nexiste pas !",u_nom);
         SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
        }
    }
    else
    {
         SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
         printf("\n!! il n'existe aucun utilisateur !");
         SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
    }
}
void add_fich(repertoire rep_tete,char repnom[],char fichnom[],char creator[]){
		repertoire p;
	    fichier f,ff;

    p=rep_tete ;
	if (rep_tete!=NULL)
	{
	  while ((p!=NULL)&&(strcmp(p->nom,repnom)!=0 ) )
	  {
	  	p=p->suiv;
	  }
	  if(p!=NULL && strcmp(p->nom,repnom)==0)
	  {
	  	f=p->fich;
	  	ff=(fichier)malloc(sizeof (struct fichiers));
	  	strcpy((ff)->nom,fichnom);
	  	strcpy((ff)->createur,creator);

        p->fich = ff;
        ff->suiv = f;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x03);
        printf("\n==> votre fichier a ete bien creer ");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
	  }
	  else
	  {
	    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
	  	printf("\n!! desole le repertoire  %s nexiste pas  :",repnom);
	  	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
	  }
	}
}
void add_rep(repertoire *rep_tete,char nom[20]) /*crieer un nouveau sous-repertoire*/ /*il faut la donner la tete de la liste des repertoire */{
    repertoire p,q;
 if (*rep_tete==NULL)
 {
 	 p=(repertoire)malloc(sizeof(struct repertoires));
 	  strcpy((p)->nom,nom);
 	  p->suiv=NULL;
 	  p->fich=NULL;
 	  *rep_tete=p;
	 }
	 else
	 {
        p=*rep_tete;
	  while (p->suiv!=NULL)
	  {
	  	p=p->suiv;
	  }
	  q=(repertoire)malloc(sizeof (struct repertoires));
	  strcpy((q)->nom,nom);
	  q->suiv=NULL;
	  q->fich=NULL;
	  p->suiv=q;
	 }
	 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x03);
	 printf("\n==> votre repertoire '%s' a ete bien creer : ",nom);
	 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
}
void fich_exist(repertoire rep,char nomrep[],char nomfich[],int *tr,char creator[]){
    repertoire pp;
    pp=rep;
    fichier ff;
    while(pp!=NULL && strcmp(pp->nom,nomrep)!=0)
    {
        pp=pp->suiv;
    }
    if (pp!=NULL)
    {
        ff=pp->fich;
        while(ff!=NULL && strcmp(ff->nom,nomfich)!=0)
        {
            ff=ff->suiv;
        }
        if(ff!=NULL &&strcmp(ff->nom,nomfich)==0 )
        {
            *tr=1;
            strcpy(creator,ff->createur);
        }
        else
        {
            *tr=0;
        }

    }
    else
    {
        *tr=0;
    }
}
void affich_rep(repertoire rep_tete){              // afficher la liste des rep avec les s-fichier
    repertoire p;
    fichier f;
    p=rep_tete;
    int cont=1,contt=1;
    if (p!=NULL)
    {while (p!=NULL)
    {
        printf("\n_repertoire %d est:{ %s }et il contient:",cont,p->nom);
        if (p->fich!=NULL)
        {
         f=p->fich;
         while(f!=NULL)
         {
             printf(" fichier %d:{ %s }",contt,f->nom);
             contt++;
             f=f->suiv;
         }
        }
        else
        {
            printf(" le vide");
        }
        cont++;

        p=p->suiv;
    }
    }
    else
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
        printf("\n!!  la liste des repertoire est vide !");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
    }
}
void affich_fich(repertoire rep_tete,char rep_nom[]){ // afficher les s-fichier d'un rep
    repertoire p;
    fichier f;
    int cont=1;
	p=rep_tete ;
	if (rep_tete!=NULL)
            {
              while ((p!=NULL)&&(strcmp(p->nom,rep_nom)!=0 ) )
              {
                p=p->suiv;
              }
              if(strcmp(p->nom,rep_nom)==0)
                      { f=p->fich;
                        if (f!=NULL)
                        {
                        while(f!=NULL)
                        {printf("\n le fichier %d est:  %s",cont,f->nom);
                        f=f->suiv;
                        cont++;
                        }
                        }
                else
                        {
                            printf("\n le repertoire %s est vide .",rep_nom);
                        }

               }
            else
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
                printf("\n!! le repertoire %s n'existe pas !",rep_nom);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
            }
            }
    else
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
        printf("\n!! il n'xiste aucun repertoire !");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
    }

}
void affich_list_users(user u){                     /*affficher la liste des user avec leur groupes*/
int i=1 ;
user q;
q=u;
while (q!=NULL){
    printf("%d-%s.\n",i,q->nom);
    q = q->suiv;
}
}
void gau(groupe g,user m){
int c;
user r;
r=m;
SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x8b);
printf("1 si oui 0 si non:\n");
SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
while(r!=NULL){
    printf("%s:\n",r->nom);
    scanf("%d",&c);
    switch(c){
    case 1:
        g_add_user(g,r);
        break;
    case 0:
        break;
    default:
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
        printf("Cette reponce n'est pas valider.\n");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
    }
    r= r->suiv;
}
}
void look_for_account(user rac,char name[20],user *u){
user q;
int found=0;
q = rac;
while((q!=NULL)&&(found==0)){
    if (strcmp(name,q->nom)==0)
        {
        found = 1;
        *u = q;
    }
    q= q->suiv;
}

}
void affiche_tab(user p_user,repertoire p_repertoire,groupe g_tete){
user u;
repertoire r;
fichier f;
char droit[20];
int max_c=0,longeur=0,nb_col=0,i;
/*trouver le nom de l'utilisateur le plus long*/
u=p_user;
while (u!=NULL)
{
    if (strlen(u->nom)>max_c){
        max_c = strlen(u->nom);
    }
    u=u->suiv;
}
nb_col+=1;
/*trouver la longeur du tableau*/
r=p_repertoire;
while(r!=NULL){
    f=r->fich;
    while(f!=NULL){
        if (strlen(f->nom)>max_c){
            max_c= strlen(f->nom);
        }
        f= f->suiv;
        nb_col+=1;
    }
    r=r->suiv;
}
max_c+=3;
longeur=max_c*nb_col;
printf("\n\n***********************************************************************************************\n\n");
printf("\n");
for (i=1;i<=longeur;i++){
    printf("-");
}
printf("\n");
for (i=1;i<=max_c-1;i++){
    printf(" ");
}
printf("|");
r=p_repertoire;
while(r!=NULL){
    f=r->fich;
    while(f!=NULL){
       printf(" %s",f->nom);
       i=strlen(f->nom)+1;
       while(i<max_c-1){
        printf(" ");
        i++;
       }
       printf("|");
        f= f->suiv;
    }
    r=r->suiv;
}
printf("\n");
for(i=1;i<=longeur;i++){
    printf("-");
}
printf("\n");
u=p_user;
while(u!=NULL){
    printf(" %s",u->nom);
       i=strlen(u->nom)+1;
       while(i<max_c-1){
        printf(" ");
        i++;
       }
    printf("|");
r=p_repertoire;
while(r!=NULL){
    f=r->fich;
    while(f!=NULL){
        u_droit_2(u,f->nom,droit,g_tete);
        if (strcmp(droit,"fff")==0)
        {
             SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
             printf(" %s",droit);
              SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
        }
        else
        {printf(" %s",droit);}
        i=4;
        while(i<max_c-1){
        printf(" ");
        i++;
       }
        printf("|");
        f= f->suiv;
    }

    r=r->suiv;
}
printf("\n");
    for(i=1;i<=longeur;i++){
    printf("-");
}
printf("\n");
    u=u->suiv;
}


printf("\n\n***********************************************************************************************\n\n");

}
int verif_name_user (user u_tete,char name[20]){
	user u;
	int tr=0;
	if (u_tete!=NULL)
	{
		u=u_tete;
		while (u!=NULL && tr!=1)
		{
			if (strcmp(u->nom,name)==0)
			{
				tr=1;
			}
			u=u->suiv;
		}
	}

	else {return 0;
	}
	if (tr==1)
	{
	    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
	printf("\n******  dezole ce nom '%s a ete deja utilise ; svp donnez un autre nom:********\n",name);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
	return 1;
	}
	else
	{return 0;
	}
}
int verif_name_user_2 (user u_tete,char name[20]){//kifkif brk hada ma yaffichiche msg
	user u;
	int tr=0;
	if (u_tete!=NULL)
	{
		u=u_tete;
		while (u!=NULL && tr!=1)
		{
			if (strcmp(u->nom,name)==0)
			{
				tr=1;
			}
			u=u->suiv;
		}
	}

	else {return 0;
	}
	if (tr==1)
	{
		return 1;
	}
	else
	{return 0;
	}
}
void g_droit(groupe g,char fichier[20],char dr[20]){
//cet fonction va nous retouner deux (2)valeurs posibles dans dr apartir du droits de groupe
//1-"ndf" (non definit):si les droit de se fichier ne sont pas specifier pour ce groupe
//2-droit de forme rwx si les droits sont definits
pr_fichier p;
p = g->rights;
int found=0;
while((p!=NULL)&(found==0)){
      if (strcmp(p->nom,fichier)==1){
        found = 1;
        strcpy(dr,p->droit);
      }
      else{
        p=p->suiv;
      }
      }
    if (found==0) {strcpy(dr,ndf);}/*non definit*/
}
void g_aff_droit(groupe g,char fich[20],char d[20]){
groupe q;
pr_fichier u;
int found=0;
u= g->rights;
while ((found==0)&&(u!=NULL)){
    if (strcmp(fich,u->nom)==0) {
        found = 1;
        strcpy(u->droit,d);
        }
        else {u = u->suiv;}
    }
if (found==0){
    g_add_fichier(g,fich,d);
}
}
void u_droit_2(user u,char f[20],char d[20],groupe g_tete){
/*cet fonction va nous retouner troit (3)valeurs posibles dans dr apartir des droits des groupe de cette utilisateur
1-"r__" (droits par defaut):si tous les droits des groupes de user "u" ne sont pas encor definit ("ndf")
2-droit de forme rwx si :
  *-Il existe des groupes ou les droits sont definit
  *-et tout les groupes qui "ont des droits definit" porte le meme droit pour ce fichier
3-"FFF" si il y on a "aumoin deux(2)" droit definit qui ne sont pas identique*/

pr_groupe pr ;
pr_fichier fg ;
char dd[4],verif[4],ddd[4],droit_perso[4];
int i=0,ii,cont2=0,cont1=0,cont3=0;
groupe gr,groupe_perso;
trouve_groupe(g_tete,u->pr_groupe->nom,&groupe_perso);
g_droit(groupe_perso,f,droit_perso);
//les droits personels sont les droits du premier groupe crier apres la creation de l'utilisateur
if (strcmp(droit_perso,"ndf")!=0){//les droits perso sont definit
    strcpy(d,droit_perso);
}
else{//les droits perso nesont pas definit:
pr = u->pr_groupe;
while (pr!=NULL)
{
   trouve_groupe(g_tete,pr->nom,&gr);
   cont1++;
   fg=gr->rights;
   while (fg!=NULL && strcmp(fg->nom,f)!=0)
   {

       fg=fg->suiv;
   }

   if (fg!=NULL && strcmp(fg->nom,f)==0)
   {
       strcpy(dd,fg->droit);
   }
   else
   {

       strcpy(dd,"ndf");
       cont2++;

   }
   if (cont1==1)
   {
       strcpy(verif,dd);
   }
   if (strcmp(dd,"ndf")!=0 && strcmp(dd,verif)!=0)
   {
       cont3++;
   }
   if (strcmp(dd,"ndf")!=0)
   {
       strcpy(ddd,dd);
   }
   pr=pr->suiv;
}
if (cont2==cont1)
{
    strcpy(d,"r__");
}
    else
    {
        if (cont2==cont1-1)
        {
            strcpy(d,ddd);
        }
        else
        {
            if(cont3>0)
            {
                strcpy(d,"fff");
            }
            else
            {
                strcpy(d,verif);
            }
        }
    }
}
}
void saut(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0e);
    printf(" \n\n===> press 'ENTRER' to continue ");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
  getchar();
    getchar();
}



int main(){
char createur[20],name[20],g_name[20],name_u[20],fichier[20],nom_racine_rep[20],nom_rep[20],dr[20];
int tr,sortir=0,contr=0,p_g_choix,p_g_choit,reponce,reponce_u,reponce_f,racine_r_ex=0,mot_passe,reponce_list_u,p_fichier=0,exist,change_m_p;
user p_user=NULL,ad_user,q,user,p;
groupe p_groupe=NULL,n_groupe,groupe; /*premier groupe et nouveau groupe*/
repertoire ad_racine_rep=NULL;
srand(time(NULL));


                                                   /* INTERFACE*/
/*________________________________________________________________________________________________________________________________*/
printf("\n\n");

SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x8f);
printf("                                                                                                                       \n");
printf("                                                      TP 01                                                            \n");
printf("                                                   DROIT D'ACCES                                                       \n");
printf("                                                                                                                       \n");
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
  printf("\n\n\n");
SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0b);
  printf("  nom prenom : \n\n");
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
  printf("_ sahbi ouael nedjem eddine ( esi 1cpi gr 02 )                                               annee scolaire :2019_2020 \n");
  printf("_ almamma amir              ( esi 1cpi gr 02 )\n");
   printf("\n\n\n");
   saut();
    printf("\n\n\n");
/*____________________________________________________________________________________________________________________________________*/





while (sortir==0){
    if (contr==0)  /* cette partie est juste pour l'dministrateur */
        {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
        printf("\n********************************               VOUS ETES NOUVEAU ISI!!             ************************************\n");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
        printf("\n________Veuillez crier votre compte (root):_______\n");
        printf("==> donnez votre nom:");
        scanf("%s",name);
        add_user(&p_user);
        creat_user(p_user,name);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x03);
        printf("\n=>your account is succesfuly created\n");
        printf("******  Welcome '%s' !!  ***** \n\n",name);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x8b);
            printf("\n__________vous pouvez____________________\n");
            printf("1- crier un groupe avec le nom '  %s  '\n",name);
            printf("2- crier un nouveau groupe               \n");
            printf("_________________________________________\n");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
            printf("\n=> Enterer votre repense (1 ou 2):");
            scanf("%d",&p_g_choit);
            if (p_g_choit==1){
                /*premier groupe*/
                //add_groupe(&p_groupe);
                crier_groupe(&p_groupe,name);
                p_groupe->perso=1; //ce groupe est personel
                g_add_user(p_groupe,p_user);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x03);
                printf("\n\n ***Creation du groupe reussite.Vous appartenez a ce groupe.\n");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
            }
            else{
                if (p_g_choit==2){
                //add_groupe(&p_groupe);
                printf("==> donnez le nom du groupe:");
                scanf("%s",g_name);
                crier_groupe(&p_groupe,g_name);
                p_groupe->perso=1; //ce groupe est personel
                g_add_user(p_groupe,p_user);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x03);
                printf("\n\n ==> Creation du groupe reussite.Vous appartenez a ce groupe.\n");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
                }
                else{
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
                    printf("\n\n !! cette reponse n'est pas valider.");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
                }
            }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
        printf("\n**** Vous etes maintenant l'adminnistrateur.****\n");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
        printf("==> votre mot de passe est:%d",p_user->ID);

        saut();
        printf("\n\n Est ce que vous voulez changer le mot de passe?\n");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x8b);
        printf("==>Entrer votre reponse(1=oui/0=non):");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
        scanf("%d",&change_m_p);
        if (change_m_p==1){
            printf("\n\n Entrer le nouveau mot de passe(ne contient que des chiffre):");
            scanf("%d",&mot_passe);
            p_user->ID=mot_passe;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x03);
            printf("\n\n***Mot de passe changer avec changer avec succee**");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
           saut();
        }
    }
    else{
      printf("\n\n\n==> qu'est ce que vous voulez faire ?\n");
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x8b);
      printf("\n___________Vous pouvez:________________\n");  /* toujour le root qui va faire ca*/
      printf("1- Inserer un nouveau compte.          \n");
      printf("2- Ajouter un nouveau Groupe.          \n");
      printf("3- Crier un fichier.                   \n");
      printf("4- Crier un repertoire.                \n");
      printf("5- Ajouter un utilisateur a un groupe. \n");
      printf("6- Affecter des droits aux groupe.     \n");
      printf("7- Affecter des droits aux utilisateurs\n");
      printf("8- supprimer un copmt.                 \n");
      printf("9- afficher la base de donner          \n");/*principallement pour tester le programme */
      printf("10- recherche d'un compt               \n");
      printf("_______________________________________\n");
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
      printf("\n\n==> Veuiller entrer votre reponse:");

      scanf("%d",&reponce);

      switch(reponce){
      case 1:       /*Insertion Nouveau compte*/
                    do
                    {
					printf("==> donnez le nom du nouveau compt:");
                    scanf("%s",name_u);    /*nom d 'utilisateur*/
                    }while(verif_name_user(p_user,name_u)!=0);
                    add_user(&ad_user);
                    creat_user(ad_user,name_u);
                    q = p_user->suiv;
                    u_aff_suiv(p_user,ad_user);
                    u_aff_suiv(ad_user,q);
                    crier_groupe(&p_groupe,name_u);
                    p_groupe->perso=1; //ce groupe est personel
                    trouve_groupe(p_groupe,name_u,&groupe);
                    look_for_account(p_user,name_u,&user);
                    g_add_user(groupe,user);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x03);
                    printf("\n**** your account is succesfuly created\n");
                    printf("Welcome %s !! \n\n",name_u);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x8b);
                    printf("\n____Voulez vous____________________________:\n");  /* user who will did this */
                    printf("1-Crier un fichier.                          \n");
                    printf("2-Crier un repertoire.                       \n");
                    printf("3-ne pas cier ni un fichier ni un repertoire.\n");
                    printf("_____________________________________________\n");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
                    printf("\n\nVeuiller entrer votre reponse:");
                    scanf("%d",&reponce_u);

                        switch(reponce_u){
                        case 1: /*creation d'un nouveau fichier*/
                            p_fichier = 1;
                            printf("\n\nDonner le nom du nouveau fichier:");
                            scanf("%s",fichier);
                            trouve_groupe(p_groupe,name_u,&groupe);
                            g_aff_droit(groupe,fichier,rw_);/*les droit personel par default*/
                           /* remarque:les droits public par default sont: r__ (read only)*/
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x8b);
                            printf("\n\n______Est ce que vous voulez metre ce fichier:_____\n");
                            printf("1- Dans un repertoire qui existe deja?               \n");
                            printf("2- dans un nouveau repertoire.                       \n");
                            printf("_____________________________________________________\n");
                             SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
                            printf("\n\n==> Veuiller entrer votre reponse:");
                            scanf("%d",&reponce_f);
                            switch (reponce_f){
                                case 1:     /*Repertoire qui existe deja*/
                                    if (racine_r_ex==0)
                                        {
                                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
                                        printf("\n!!!! Desole y on a pas encor de repertoire.Il faut crier le repertoire:\n");
                                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
                                        printf("\nDonnez le nom du nouveau repertoire:");
                                        scanf("%s",nom_racine_rep);/*nom de la racine des repertoies*/
                                        add_rep(&ad_racine_rep,nom_racine_rep);
                                        racine_r_ex = 1;/*racine repertoire existe*/
                                        add_fich(ad_racine_rep,nom_racine_rep,fichier,name_u);

                                        }
                                    else{
                                        printf("\n_________la liste des repertoire qui existe est_______ :");
                                        affich_rep(ad_racine_rep);
                                        printf("\n _______________________________________________________");
                                        /*affichir la liste des repertoires pour que l'utilisateur peut choisir*/

                                        printf("\n==> Entrer le nom du nouveau repertoire:");
                                        scanf("%s",nom_rep);
                                        add_fich(ad_racine_rep,nom_rep,fichier,name_u);
                                        }

                                 break;
                                 case 2:/*creation d'un nouveau fichier dans un nouveau repertoire */
                                   printf("\n ===> donner le nom du nouveau repertoire :");
                                   scanf("%s",nom_rep);
                                   add_rep(&ad_racine_rep,nom_rep);
                                   add_fich(ad_racine_rep,nom_rep,fichier,name_u);
                                   racine_r_ex = 1;
                                 break;
                                    }
                  break;
                         case 2:

                            /*creation d'un nouveau repertoire*/
                            printf("\n==> donner le nom du nouveau repertoire :");
                                    scanf("%s",nom_rep);
                                    add_rep(&ad_racine_rep,nom_rep);
                                    racine_r_ex = 1;

                 break;
                         case 3:
                            break;

            }
               saut();
      break;
      case 2:
      /*Ajout de groupe*/
      printf("!! Veuillez confirmer que voue etes l'administrateur:\n");
      printf("==> Donnez le mot de passe:");
      scanf("%d",&mot_passe);
      if (mot_passe==p_user->ID){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x03);
        printf("\n\n***** Welcome Back '%s' !! ****\n\n",p_user->nom);
         SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
        if (p_groupe!=NULL){/*le premier groupe deja existe*/
        printf("==> donnez le nom du nouveau groupe:");
        scanf("%s",g_name);
        //add_groupe(&p_groupe);
        crier_groupe(&p_groupe,g_name);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x8b);
        printf("\n__________Voulez vous laisser ce groupe vide__________________\n");
        printf("1-Oui je veut laisser ce groupe vide                            \n");
        printf("2-Non je veut la lists des utilisateur pour ajouter a ce groupe.\n");
        printf("________________________________________________________________\n");
         SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
        printf("==> Entrer votre reponse:");
        scanf("%d",&reponce_list_u);
        switch(reponce_list_u){
            case 1:
                break;
            case 2:
                printf("_________Les utilisateurs qui existe sont:__________\n");
                afficher_user(p_user);
                printf("\n___________________________________________________\n");
                printf("\n==> Veuillez selectioner les utilisateurs a ajouter:\n");
                getchar();
                gau(p_groupe,p_user);/*groupe add users*/
                printf("_________la liste des utilisateur apres l'addition est:_______\n");
                afficher_user(p_user);
                printf("\n_____________________________________________________________\n");
                break;
            default:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
                printf("!!! Cette reponse n'est pas valider.\n");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x03);
        printf("\n\n==> Creation du groupe reussite.\n");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
      }
      else{
         printf("=>donnez le nom du groupe:");
        scanf("%s",g_name);
        //add_groupe(&p_groupe);
        crier_groupe(&p_groupe,g_name);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x8b);
        printf("\n___________Voulez vous laisser ce groupe vide_________________\n");
        printf("1-Oui je veut laisser ce groupe vide.                           \n");
        printf("2-Non je veut la lists des utilisateur pour ajouter a ce groupe.\n");
         printf("_______________________________________________________________\n");
         SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
        printf("==> Entrer votre reponse:");
        scanf("%d",&reponce_list_u);
        switch(reponce_list_u){
            case 1:
                break;
            case 2:
                printf("_____________Les utilisateurs qui existe sont:______________\n");
                 affich_list_users(p_user);
                 printf("\n__________________________________________________________\n");
                printf("\nVeuillez selectioner les utilisateurs a ajouter:\n");
                gau(p_groupe,p_user);/*groupe add users*/
                break;
            default:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x8b);
                printf("!!! Cette reponse n'est pas valider.\n");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x03);
        printf("\n\n==> Creation du groupe reussite.\n");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
      }
      }
      else{
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
        printf("\n !!!! Mot de passe incorecte.\n");
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
      saut();
      }
      break;
      case 3: /* un user va crier un nouveau fichier */
          do{
            printf("==>Donnez la nom de l'utilisateur qui va creer ce nouveau fichier:");
          scanf("%s",name_u);
          exist= verif_name_user_2(p_user,name_u);
          if (exist==0)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
                printf("\n cet utilisateur n'exist pas veullez reessayez. \n\n");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
            }
          }while(exist!=1);
          if (strcmp(p_user->nom,name_u)!=1){
            printf("\nVoulez confirmer que vous etes l'admin.\n");
            printf("==>Entere le mot de passe:");
            scanf("%d",&mot_passe);
            if (mot_passe==p_user->ID){
        trouve_groupe(p_groupe,name_u,&groupe);

        p_fichier = 1;
        printf("\n\nDonner le nom du fichier:");
        scanf("%s",fichier);
        g_aff_droit(groupe,fichier,rw_); /* affectation des droit personnalisé aux user qui creer ce fichier */
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x8b);
        printf("\n\n_________Est ce que vous voulez metre ce fichier:________\n");
        printf("1- Dans un repertoire qui existe deja                      \n");
        printf("2- dans un nouveau repertoire.                             \n");
        printf("___________________________________________________________\n");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
        printf("\n\n==> Veuiller entrer votre reponse:");
        scanf("%d",&reponce_f);
        switch (reponce_f){
            case 1:     /*Repertoire qui existe deja*/
                if (racine_r_ex==0)
                    {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
                printf("\n!!! Desole y on a pas encor de repertoire.Il faut crier le repertoire:\n");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
                printf("\n==> Donnez le nom du repertoire:");
                scanf("%s",nom_racine_rep);/*nom de la racine des repertoies*/
                add_rep(&ad_racine_rep,nom_racine_rep);
                add_fich(ad_racine_rep,nom_racine_rep,fichier,name_u);


                                            }
                else{
                printf("\n ___________la liste des repertoire qui existe est :___________");
                affich_rep(ad_racine_rep);
                 printf("\n_______________________________________________________________\n");
                /*affichir la liste des repertoires pour que l'utilisateur peut choisir*/
                printf("\n===> Entrer le nom du  repertoire:");
                scanf("%s",nom_rep);
                add_fich(ad_racine_rep,nom_rep,fichier,name_u);
                }
                racine_r_ex = 1;/*racine repertoire existe*/


                break;
                case 2: /*creation d'un nouveau fichier dans un nouveau repertoire */
                printf("\n==> donner le nom du nouveau repertoire :");
                scanf("%s",nom_rep);
                add_rep(&ad_racine_rep,nom_rep);
                add_fich(ad_racine_rep,nom_rep,fichier,name_u);
                racine_r_ex = 1;
                break;
                default:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
                printf("\n!!! Cette reponce n'est pas valider.\n");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
                break;
            }
            }
            else{
               SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
                printf("\n\n!! Mot de passe incorrect.\n");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
            }
          }
          else{

             trouve_groupe(p_groupe,name_u,groupe);

        p_fichier = 1;
      printf("\n\nDonner le nom du fichier:");
      scanf("%s",fichier);
      g_aff_droit(groupe,fichier,rw_);
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x8b);
        printf("\n\n_________Est ce que vous voulez metre ce fichier:________\n");
        printf("1- Dans un repertoire qui existe deja?                     \n");
        printf("2- dans un nouveau repertoire.                             \n");
        printf("___________________________________________________________\n");
         SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
        printf("\n\n==> Veuiller entrer votre reponse:");
        scanf("%d",&reponce_f);
        switch (reponce_f){
            case 1:     /*Repertoire qui existe deja*/
                if (racine_r_ex==0)
                    {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
                printf("\n!!! Desole y on a pas encor de repertoire.Il faut crier le repertoire:\n");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
                printf("\n==> Donnez le nom du repertoire:");
                scanf("%s",nom_racine_rep);/*nom de la racine des repertoies*/
                add_rep(&ad_racine_rep,nom_racine_rep);
                add_fich(ad_racine_rep,nom_racine_rep,fichier,name_u);

                                            }
                else{
                printf("\n ___________la liste des repertoire qui existe est :___________");
                affich_rep(ad_racine_rep);
                 printf("\n_______________________________________________________________\n");
                /*affichir la liste des repertoires pour que l'utilisateur peut choisir*/
                printf("\n===> Entrer le nom du repertoire:");
                scanf("%s",nom_rep);
                add_fich(ad_racine_rep,nom_rep,fichier,name_u);
                }
                racine_r_ex = 1;/*racine repertoire existe*/


                break;
                case 2: /*creation d'un nouveau fichier dans un nouveau repertoire */
                printf("\n==> donner le nom du nouveau repertoire :");
                scanf("%s",nom_rep);
                add_rep(&ad_racine_rep,nom_rep);
                add_fich(ad_racine_rep,nom_rep,fichier,name_u);
                racine_r_ex = 1;
                break;
                default:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
                printf("\n!!! Cette reponse n'est pas valider.\n");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
                break;
            }
          }

      break;
      case 4:/*l'utilisateur va creer un nouveau repertoire */
        printf("\n==> donner le nom du nouveau repertoire :");
        scanf("%s",nom_rep);
        add_rep(&ad_racine_rep,nom_rep);
        racine_r_ex = 1;

      break;
      case 5:
      /*ajout d'un utilisateur a un groupe*/
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x8b);
      printf("__________Est ce que vous voulez:________________________\n");
      printf("1-Crier un nouveau et ajouter le a un groupe existant.   \n");
      printf("2-Ajouter un des utilisateurs qui exite deja a un groupe.\n");
      printf("_________________________________________________________\n");
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
      printf("\n=>Veullez entrer votre reponse:");
      scanf("%d",&reponce);
      switch(reponce){
        case 1:
        do
                    {
					printf("==> donnez le nom du nouveau compt:");
                    scanf("%s",name_u);    /*nom d 'utilisateur*/
                    }while(verif_name_user(p_user,name_u)!=0);
                    add_user(&ad_user);
                    creat_user(ad_user,name_u);
                    q = p_user->suiv;
                    u_aff_suiv(p_user,ad_user);
                    u_aff_suiv(ad_user,q);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x03);
                    printf("\n**** your account is succesfuly created\n");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
                    getchar();
      if (p_groupe!=NULL){
        printf("___________La list des groupe qui exist est:____________\n");
      affiche_groupe(p_groupe);
       printf("\n__________________________________________________________\n");
      printf("\n\nEntrer le nom du groupe:");
      scanf("%s",g_name);
      trouve_groupe(p_groupe,g_name,&groupe);
      if (groupe->perso ==1){
        printf("\nCe groupe est personel.");
      }
      else{
        g_add_user(groupe,ad_user);
      }
      }
      else{
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
        printf("!!! Y on a pas encor de groupe .Il faut crier le premier.");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
        printf("\n\n==> Entrer le nom du groupe:");
        scanf("%s",g_name);
        //add_groupe(&groupe);
        crier_groupe(&p_groupe,g_name);
        g_add_user(p_groupe,user);
      }
      printf("=> Done!");
      getchar();
      break;
        case 2:
            printf("____________ les utilisateur qui existes sont:___________\n");
            afficher_user(p_user);
             printf("\n________________________________________________________\n");
            getchar();
            printf("____________Les groupes qui existe sont:___________________");
            affiche_groupe(p_groupe);
            printf("\n________________________________________________________\n");
            getchar();
            if (p_groupe!=NULL)
            {
            printf("\n\n=> Entrer le nom du groupe:");
            scanf("%s",g_name);
            trouve_groupe(p_groupe,g_name,&groupe);
            printf("\n=> Donnez le nom de l'utilisateur:");
            scanf("%s",name_u);
            look_for_account(p_user,name_u,&user);
            g_add_user(groupe,user);
            }
            else
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
                printf("\n!! il nexiste aucun groupe ");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
                printf("\n ==> creer le premier ");
                printf("\n\n==> Entrer le nom du groupe:");
                scanf("%s",g_name);
                //add_groupe(&p_groupe);
                crier_groupe(&p_groupe,g_name);
                printf("\n=> Donnez le nom de l'utilisateur:");
                scanf("%s",name_u);
                look_for_account(p_user,name_u,&user);
                g_add_user(p_groupe,user);
            }
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x03);
            printf("==> Utilisateur %s ajouter a groupe %s avec succer.",user->nom,groupe->nom);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);

            break;

      }
      break;
      case 6:
      /*manipulation des droits*/
         if (p_fichier==1){
          printf("\n\n ___________la liste des groupes crees par l'administrateur est :__________\n");
          affiche_groupe(p_groupe);
          printf("\n________________________________________________________________________________\n");
          printf("\n\n _____________la liste des utilisateur crees par l'administrateur est________ :\n");
          afficher_user(p_user);
          printf("\n________________________________________________________________________________\n");
          printf("\n\n _____________la liste des repertoires avec les s-fichers est:_________________\n");
          affich_rep(ad_racine_rep);
           printf("\n________________________________________________________________________________\n");
          printf("\n\n==> Entrer Le nom de groupe:");
          scanf("%s",g_name);
          printf("\n==> Entrer Le nom du fichier:");
          scanf("%s",fichier);
          printf("==> Entrer les droits(forme rwx):");
          scanf("%s",dr);
          trouve_groupe(p_groupe,g_name,&groupe);
          g_aff_droit(groupe,fichier,dr);
          SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x03);
          printf("\n==> Droits affecter avec succee.");
          SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F) ;
          getchar();
           }
           else{
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
            printf("!!! Y on a pas encor de fichier.");
             SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
           }
      break;
      case 7:
           /*affecter des droits au utilisateurs */
           printf ("+> REMARQUE : juste le root ou l'utilisateur qui a creer le fichier qui peut affecter des droit au utisateurs\n");
           printf ("_ donner votre nom: ");
           scanf("%s",name_u);
           if(verif_name_user_2(p_user,name_u)!=0)
           {
             printf("\n ** welcome '%s'",name_u);
                if (ad_racine_rep!=NULL)
                 {
                         printf("\n________________ la liste des rep et des fichier qui existe deja:________________\n");
                         affich_rep(ad_racine_rep);
                         printf("\n__________________________________________________________________________________\n");
                         printf("\n=> donner le nom du repertoire puis le nom du  fichier\n");
                         printf(" rep:");
                         scanf("%s",nom_rep);
                         printf("\nfichier:");
                         scanf("%s",fichier);
                         fich_exist(ad_racine_rep,nom_rep,fichier,&tr,createur);
                         if (tr==1)
                         {
                                if (strcmp(name_u,createur)==0 || strcmp(name_u,p_user->nom)==0)
                                {
                                      printf("\n==> Entrer les droits(forme rwx):");
                                      scanf("%s",dr);
                                      trouve_groupe(p_groupe,name_u,&groupe);
                                      g_aff_droit(groupe,fichier,dr);
                                      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x03);
                                      printf("\n==> Droits affecter avec succee.");
                                      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F) ;
                                 }
                                 else
                                    {   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0c);
                                        printf("\n !! dezoler mais vous etes ni le root ni le createur de ce fichier");
                                         SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
                                    }

                         }
                         else
                         {SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0c);
                            printf(" !! ce fichier '%s' n'existe pas dans le repertoire '%s' !! ",fichier,nom_rep);
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
                         }
               }
               else
               {SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0c);
                   printf("!! dezole il nexiste aucun repertoire encore !!\n");
                   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
               }
           }
           else
           {SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0c);
               printf("=> !!! dezole mais usre '%s' n'xiste pas \n",name_u);
               SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
           }
        break;
      case 8:
          /*supprimer un compt*/
          printf("\n\n ____________la liste des utilisateur crees par l'administrateur est :________");
          afficher_user(p_user);
           printf("\n________________________________________________________________________________\n");
          printf("\n\n==> donner le nom du compt que vous voulez suprimer :");
          scanf("%s",name_u);
          if (strcmp(name_u,name)==0)
          {
               SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
              printf("\n!!! tu ne peut pas suprimer l'administrateur");
               SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
          }
          else
          sup_user(&p_user,name_u,p_groupe);
        break;
      case 9:
          /* afficher la base de donner*/
          printf("\n\n ___________la liste des groupes crees par l'administrateur est :__________\n");
          affiche_groupe(p_groupe);
          printf("\n________________________________________________________________________________\n");
          printf("\n\n _____________la liste des utilisateur crees par l'administrateur est________ :\n");
          afficher_user(p_user);
          printf("\n________________________________________________________________________________\n");
          printf("\n\n _____________la liste des repertoires avec les s-fichers est:_________________\n");
          affich_rep(ad_racine_rep);
           printf("\n________________________________________________________________________________\n");
        break;
      case 10:
          /* recherche d'un compt*/
          printf("\n==> donner le nom du compt: ");
          scanf("%s",name_u);
          rech_compt(p_user,name_u);
        break;

      default:
           SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
          printf("\n!!! Cette reponce n'est pas valider.\n");
           SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
      }
    }
    if (contr!=0){
    printf("\n\n\n\n-------- voulez vous sortir du programme? --------\n");
     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x8b);
    printf("====>   si oui entrer '1' sinon entere '0':");
     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
    scanf("%d",&sortir);
    }
    contr++;
}
           SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x03);
          printf("\n _________________________________   la matrice  _________________________________");
           SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
           affiche_tab(p_user,ad_racine_rep,p_groupe);
SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
           printf ("\n_______________________!!  REMARQUE IMPORTANTE CONCERNAT LA MATRICE !! _____________________________________________");
           SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
           printf("\n===> si la case de la matrice contient un droit de 'rwx' signifie que :\n ");
                   printf("    1- il existe un droit personnalise de lutisateur ver ce fichier qui est plus preioritere \n");
                   printf("OU   2- tous les droit qui sont affecté au groupes de ce utilisateur sont iddentique          \n");
           printf("\n===> si il ya une ' ");
           SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0c);
           printf("fff");
           SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
           printf(" ' alors il existe un conflit est le conflit c'est lorsque : \n ");
                   printf("      _il existe au moins deux droit qui sont affecté au groupes de ce user qui sont different \n en plus ce user n'a pas des doits personalise pour ce fichier\n");


return 0;
}


