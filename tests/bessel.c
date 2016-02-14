/*
 * Copyright (c) 2002, 2016 Jens Keiner, Stefan Kunis, Daniel Potts
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/* Standard headers. */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <CUnit/CUnit.h>

#include "nfft3.h"
#include "infft.h"
#include "bessel.h"

static const R r[] =
{
  K(1.),
  K(1.266065877752008335598244625214717537607670311354962206808135331),
  K(2.279585302336067267437204440811533353285841102785459054070839752),
  K(4.880792585865024085611235546021319249424328856405997616059563816),
  K(11.30192195213633049635627018321710249741261659443533770600649619),
  K(27.23987182360444689454423207588441928247906183222098381517165811),
  K(67.23440697647797532618802514487659014539309769967631228874757693),
  K(168.5939085102896988573266271875008403765226792345317141931940557),
  K(427.5641157218047851773967913180828851255641047912371158842972189),
  K(1093.588354511374695845824251425470842609463932161735715101482777),
  K(2815.71662846625447146981115342659009307845123839607778216714826),
  K(7288.489339821248106181443961076268087674263267653243872277051542),
  K(18948.92534929630886120810822975396610096395087020475178444194212),
  K(49444.4895822175729992545976985628670746866673348371619760370017),
  K(129418.5627006485636645883284779364697034506653868683440024006502),
  K(339649.3732979138795217016299149152510183515032946362766302471709),
  K(893446.2279201050170708640309761884542780698188588532984237189283),
  K(2.354970223168293373199392332992929747499669855367279146087662349e6),
  K(6.218412420781002949862477755522804357608567431075431079140885033e6),
  K(1.644619044061171082202060461873027324489627712325221659994963911e7),
  K(4.355828255955353327210666008921769191706709948274652699301682074e7),
  K(1.155139619221580641210055580833193182349008358683329204624917575e8),
  K(3.066929936403647456134989153557154852040468018913248584558728394e8),
  K(8.151421225128922565171561357437946877416833345683465070459333522e8),
  K(2.168619088241376503688993617666420248551029614224983674396580205e9),
  K(5.774560606466310315771339797306938266433245397873936700455049101e9),
  K(1.53889767056608104628354178833613609965823297928610316806121827e10),
  K(4.104200585475795553140327272390885136563132067541576482534974362e10),
  K(1.095346047317572734826748564401226073645732030800725365612567578e11),
  K(2.925206317856908662657116410293661481220676238939614836587072399e11),
  K(7.816722978239774897173898167052950054449442539779470033476887726e11),
  K(2.089962966491903795438112890993681402263449110349676238965437089e12),
  K(5.590908381350873086500103776088605327594330420659633252120139658e12),
  K(1.496379690415690565191814638473178109312332923322021932559495447e13),
  K(4.0068575042540716033612386491417848375483205750833823294989207e13),
  K(1.073388184945140635734893838464930548254210589870069038147915287e14),
  K(2.876667778112440429102939018743956743168597811585388259920143996e14),
  K(7.712455222928105049470387028786917387436448434634188854693758863e14),
  K(2.068504714851044845188315030265960321539502797950162891330469035e15),
  K(5.549742955314183764928385386334394109192543969981657372066962891e15),
  K(1.48947747934198999242245915707211844449466595566709917645085521e16),
  K(3.998826282831435597505844155816141234868515406251494370527209842e16),
  K(1.073895413680451019816822994457615112605655249605048907233546246e17),
  K(2.884802548590664595765772630141341158469461953875893654313773634e17),
  K(7.751559537842989740397780668696425957540590900893385159036597068e17),
  K(2.083414075177314816175748879721006850740042802685317811855913058e18),
  K(5.601064760357453441253288431922714610935756843087707673641028089e18),
  K(1.506154051104215054394880799275018149574868497690218864652256885e19),
  K(4.051049958561881502403150208528714832868499711626942014609943758e19),
  K(1.08983584160280871742529301224217160362225800814598866772484422e20),
  K(2.932553783849336326654675079456853858051295754348464637687510971e20),
  K(7.892573914864747628859691581460674812767894756747969277028909921e20),
  K(2.12459265487660652056871806315958339727216109963237958243900645e21),
  K(5.720234198352232790334648646673038414994667273849501319795982319e21),
  K(1.540387598056586022981882040602949012236539330144407280660208615e22),
  K(4.148789560733177775711966584844581045554582971157758177772297083e22),
  K(1.117597102908680557440252330312308169878006518727217928273409229e23),
  K(3.011057285402750844254905449919691113803883064535375009073890045e23),
  K(8.113723742037748511430594951641382362967922457155325146313559123e23),
  K(2.186686615847541129107989239951224110687022753263248916557364992e24),
  K(5.894077055609801168278817440333904737978983020316213949659565109e24),
  K(1.588934206611165724819385952343622493754123488150542792646030561e25),
  K(4.284053401489351373487689539590410365728846884537220296619345327e25),
  K(1.155209631701217253769264865575568811024866424185109147327096053e26),
  K(3.115457918187897557650694682306942275028270357926158832387382537e26),
  K(8.403039845625431814330407809138226251834590191703526139641942355e26),
  K(2.266745504580402482346612567083138715657662968786714445572829081e27),
  K(6.115322228329697487120491748473756806457519397750500297493418396e27),
  K(1.650002782141142080206219328914127394026116255252557541946408918e28),
  K(4.452432317970361891654934322530279034245542779628151603186076768e28),
  K(1.201588957912546360498375395967869129353733632850675094875133012e29),
  K(3.243091297746846963956376519395246164201892975208344865804206702e29),
  K(8.753985270944612432992459963519116073415315421938008883059037247e29),
  K(2.363168219162483471104054116304339476639286965580861887439359919e30),
  K(6.380056142918597534977429545519136072702305046180553281007861394e30),
  K(1.722639078035804682482269501155973485723379171937369601101899288e31),
  K(4.651606402086077743494549469956883405890265150856115920007394808e31),
  K(1.256173058097408646403325613863342561020808148088469867415933456e32),
  K(3.392601579325684183940206288406749310455895248442554580118730353e32),
  K(9.163305649751018958805889165173475090032352019522042643935971216e32),
  K(2.475178404334170488669557055604816741023037025797109181730474962e33),
  K(6.686440578105684544124828193740541433177631407252035460717839412e33),
  K(1.806411872857697017039735673179409600758257628613750697833410277e34),
  K(4.880575843818504654955600081311594074858489878499938077728909919e34),
  K(1.318733593825985352694048063551898085336352557787738371327431147e35),
  K(3.563477630408124112456386583509675981295125396365027859172323093e35),
  K(9.629888111716449119891609199747156897132776193136573093246999531e35),
  K(2.602543404422873394653370151078579731417701173457461289633354059e36),
  K(7.034019697322303775839744667527863794825432576041470228870590053e36),
#ifndef NFFT_SINGLE
  K(1.901241929906489804817867678014679730480380284178119002890984985e37),
  K(5.139238345508663835278491143551074437225477975963566786265569375e37),
  K(1.389271406098962184295136044360963162648448300460678037021151849e38),
  K(3.755794357405601907191452619533778214646739013457777677287809344e38),
  K(1.015412047323622861123458506128318979949439049328250120326095335e39),
  K(2.745415388947168612088253534423581168638303920667762206373930911e39),
  K(7.423325861875348588563552460557813534887687445879568607405090463e39),
  K(2.007304126311214310046362681359609158353334647885749594503448292e40),
  K(5.428145964601007525699719297026126037833477055897514245121468102e40),
  K(1.467956067407419253935815774834718367952993875125561963795542585e41),
  K(3.970062351772964342800951229310239389328999942632802037502762997e41)
#endif
};

#define ERR(x,y) IF(ABS(x - y) == K(0.0), ABS(x - y), ABS(x - y) / ABS(y))

#if defined(NFFT_LDOUBLE)
static const R bound = K(59.0) * EPSILON;
#elif defined(NFFT_SINGLE)
static const R bound = K(14.0) * EPSILON;
#else
static const R bound = K(4.0) * EPSILON;
#endif

void X(check_bessel_i0)(void)
{
  R x = K(0.0);
  R err = K(0.0);
  unsigned int j;
  int ok;

  printf("BESSEL I0\n---------\n");

  for (j = 0; j < sizeof(r)/sizeof(r[0]); j++, x += K(1.0))
  {
    R y = X(bessel_i0)(x);
    R yr = r[j];
    err = ERR(y,yr);
    ok = IF(err < bound, 1, 0);
    fprintf(stderr, "i0[" __FE__ "] = " __FE__ " err_rel = " __FE__ " %-2s " __FE__ " -> %-4s\n", x, y,
      err, IF(ok == 0, ">=", "<"), bound , IF(ok == 0, "FAIL", "OK"));
    CU_ASSERT(ok == 1);
  }
}
