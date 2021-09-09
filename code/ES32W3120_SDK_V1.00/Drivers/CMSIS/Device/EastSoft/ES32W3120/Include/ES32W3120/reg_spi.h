/**
  **************************************************************************************
  * @file    REG_SPI.h
  * @brief   SPI Head File
  *
  * @version V0.01
  * @data    8/17/2020
  * @author  Eastsoft MCU Software Team
  * @note
  *
  * Copyright (C) 2018 Shanghai Eastsoft Microelectronics Co., Ltd. ALL rights reserved.
  *
  **************************************************************************************
  */

#ifndef __SPI_H__
#define __SPI_H__


/******************************************************************************/
/*                              设备特殊寄存器结构定义                        */
/******************************************************************************/

/*   允许匿名结构和匿名联合    */
 

/****************** Bit definition for SPI_CON1 register ************************/

#define	SPI_CON1_BIDEN_POS	15U 
#define	SPI_CON1_BIDEN_MSK	BIT(SPI_CON1_BIDEN_POS)

#define	SPI_CON1_BIDOEN_POS	14U 
#define	SPI_CON1_BIDOEN_MSK	BIT(SPI_CON1_BIDOEN_POS)

#define	SPI_CON1_CRCEN_POS	13U 
#define	SPI_CON1_CRCEN_MSK	BIT(SPI_CON1_CRCEN_POS)

#define	SPI_CON1_NXTCRC_POS	12U 
#define	SPI_CON1_NXTCRC_MSK	BIT(SPI_CON1_NXTCRC_POS)

#define	SPI_CON1_FLEN_POS	11U 
#define	SPI_CON1_FLEN_MSK	BIT(SPI_CON1_FLEN_POS)

#define	SPI_CON1_RXO_POS	10U 
#define	SPI_CON1_RXO_MSK	BIT(SPI_CON1_RXO_POS)

#define	SPI_CON1_SSEN_POS	9U 
#define	SPI_CON1_SSEN_MSK	BIT(SPI_CON1_SSEN_POS)

#define	SPI_CON1_SSOUT_POS	8U 
#define	SPI_CON1_SSOUT_MSK	BIT(SPI_CON1_SSOUT_POS)

#define	SPI_CON1_LSBFST_POS	7U 
#define	SPI_CON1_LSBFST_MSK	BIT(SPI_CON1_LSBFST_POS)

#define	SPI_CON1_SPIEN_POS	6U 
#define	SPI_CON1_SPIEN_MSK	BIT(SPI_CON1_SPIEN_POS)

#define	SPI_CON1_BAUD_POSS	3U 
#define	SPI_CON1_BAUD_POSE	5U 
#define	SPI_CON1_BAUD_MSK	BITS(SPI_CON1_BAUD_POSS,SPI_CON1_BAUD_POSE)

#define	SPI_CON1_MSTREN_POS	2U 
#define	SPI_CON1_MSTREN_MSK	BIT(SPI_CON1_MSTREN_POS)

#define	SPI_CON1_CPOL_POS	1U 
#define	SPI_CON1_CPOL_MSK	BIT(SPI_CON1_CPOL_POS)

#define	SPI_CON1_CPHA_POS	0U 
#define	SPI_CON1_CPHA_MSK	BIT(SPI_CON1_CPHA_POS)

/****************** Bit definition for SPI_CON2 register ************************/

#define	SPI_CON2_RXFTH_POSS	14U 
#define	SPI_CON2_RXFTH_POSE	15U 
#define	SPI_CON2_RXFTH_MSK	BITS(SPI_CON2_RXFTH_POSS,SPI_CON2_RXFTH_POSE)

#define	SPI_CON2_TXFTH_POSS	12U 
#define	SPI_CON2_TXFTH_POSE	13U 
#define	SPI_CON2_TXFTH_MSK	BITS(SPI_CON2_TXFTH_POSS,SPI_CON2_TXFTH_POSE)

#define	SPI_CON2_FRF_POS	4U 
#define	SPI_CON2_FRF_MSK	BIT(SPI_CON2_FRF_POS)

#define	SPI_CON2_NSSP_POS	3U 
#define	SPI_CON2_NSSP_MSK	BIT(SPI_CON2_NSSP_POS)

#define	SPI_CON2_NSSOE_POS	2U 
#define	SPI_CON2_NSSOE_MSK	BIT(SPI_CON2_NSSOE_POS)

#define	SPI_CON2_TXDMA_POS	1U 
#define	SPI_CON2_TXDMA_MSK	BIT(SPI_CON2_TXDMA_POS)

#define	SPI_CON2_RXDMA_POS	0U 
#define	SPI_CON2_RXDMA_MSK	BIT(SPI_CON2_RXDMA_POS)

/****************** Bit definition for SPI_STAT register ************************/

#define	SPI_STAT_RXFLV_POSS	24U 
#define	SPI_STAT_RXFLV_POSE	28U 
#define	SPI_STAT_RXFLV_MSK	BITS(SPI_STAT_RXFLV_POSS,SPI_STAT_RXFLV_POSE)

#define	SPI_STAT_TXFLV_POSS	16U 
#define	SPI_STAT_TXFLV_POSE	20U 
#define	SPI_STAT_TXFLV_MSK	BITS(SPI_STAT_TXFLV_POSS,SPI_STAT_TXFLV_POSE)

#define	SPI_STAT_BUSY_POS	15U 
#define	SPI_STAT_BUSY_MSK	BIT(SPI_STAT_BUSY_POS)

#define	SPI_STAT_CHSIDE_POS	14U 
#define	SPI_STAT_CHSIDE_MSK	BIT(SPI_STAT_CHSIDE_POS)

#define	SPI_STAT_RXTH_POS	12U 
#define	SPI_STAT_RXTH_MSK	BIT(SPI_STAT_RXTH_POS)

#define	SPI_STAT_RXUD_POS	11U 
#define	SPI_STAT_RXUD_MSK	BIT(SPI_STAT_RXUD_POS)

#define	SPI_STAT_RXOV_POS	10U 
#define	SPI_STAT_RXOV_MSK	BIT(SPI_STAT_RXOV_POS)

#define	SPI_STAT_RXF_POS	9U 
#define	SPI_STAT_RXF_MSK	BIT(SPI_STAT_RXF_POS)

#define	SPI_STAT_RXE_POS	8U 
#define	SPI_STAT_RXE_MSK	BIT(SPI_STAT_RXE_POS)

#define	SPI_STAT_TXTH_POS	4U 
#define	SPI_STAT_TXTH_MSK	BIT(SPI_STAT_TXTH_POS)

#define	SPI_STAT_TXUD_POS	3U 
#define	SPI_STAT_TXUD_MSK	BIT(SPI_STAT_TXUD_POS)

#define	SPI_STAT_TXOV_POS	2U 
#define	SPI_STAT_TXOV_MSK	BIT(SPI_STAT_TXOV_POS)

#define	SPI_STAT_TXF_POS	1U 
#define	SPI_STAT_TXF_MSK	BIT(SPI_STAT_TXF_POS)

#define	SPI_STAT_TXE_POS	0U 
#define	SPI_STAT_TXE_MSK	BIT(SPI_STAT_TXE_POS)

/****************** Bit definition for SPI_DATA register ************************/

#define	SPI_DATA_DATA_POSS	0U 
#define	SPI_DATA_DATA_POSE	15U 
#define	SPI_DATA_DATA_MSK	BITS(SPI_DATA_DATA_POSS,SPI_DATA_DATA_POSE)

/****************** Bit definition for SPI_CRCPOLY register ************************/

#define	SPI_CRCPOLY_CRCPOLY_POSS	0U 
#define	SPI_CRCPOLY_CRCPOLY_POSE	15U 
#define	SPI_CRCPOLY_CRCPOLY_MSK	BITS(SPI_CRCPOLY_CRCPOLY_POSS,SPI_CRCPOLY_CRCPOLY_POSE)

/****************** Bit definition for SPI_RXCRC register ************************/

#define	SPI_RXCRC_RXCRC_POSS	0U 
#define	SPI_RXCRC_RXCRC_POSE	15U 
#define	SPI_RXCRC_RXCRC_MSK	BITS(SPI_RXCRC_RXCRC_POSS,SPI_RXCRC_RXCRC_POSE)

/****************** Bit definition for SPI_TXCRC register ************************/

#define	SPI_TXCRC_TXCRC_POSS	0U 
#define	SPI_TXCRC_TXCRC_POSE	15U 
#define	SPI_TXCRC_TXCRC_MSK	BITS(SPI_TXCRC_TXCRC_POSS,SPI_TXCRC_TXCRC_POSE)

/****************** Bit definition for SPI_I2SCFG register ************************/

#define	SPI_I2SCFG_I2SMOD_POS	11U 
#define	SPI_I2SCFG_I2SMOD_MSK	BIT(SPI_I2SCFG_I2SMOD_POS)

#define	SPI_I2SCFG_I2SE_POS	10U 
#define	SPI_I2SCFG_I2SE_MSK	BIT(SPI_I2SCFG_I2SE_POS)

#define	SPI_I2SCFG_I2SCFG_POSS	8U 
#define	SPI_I2SCFG_I2SCFG_POSE	9U 
#define	SPI_I2SCFG_I2SCFG_MSK	BITS(SPI_I2SCFG_I2SCFG_POSS,SPI_I2SCFG_I2SCFG_POSE)

#define	SPI_I2SCFG_PCMSYNC_POS	7U 
#define	SPI_I2SCFG_PCMSYNC_MSK	BIT(SPI_I2SCFG_PCMSYNC_POS)

#define	SPI_I2SCFG_I2SSTD_POSS	4U 
#define	SPI_I2SCFG_I2SSTD_POSE	5U 
#define	SPI_I2SCFG_I2SSTD_MSK	BITS(SPI_I2SCFG_I2SSTD_POSS,SPI_I2SCFG_I2SSTD_POSE)

#define	SPI_I2SCFG_CKPOL_POS	3U 
#define	SPI_I2SCFG_CKPOL_MSK	BIT(SPI_I2SCFG_CKPOL_POS)

#define	SPI_I2SCFG_DATLEN_POSS	1U 
#define	SPI_I2SCFG_DATLEN_POSE	2U 
#define	SPI_I2SCFG_DATLEN_MSK	BITS(SPI_I2SCFG_DATLEN_POSS,SPI_I2SCFG_DATLEN_POSE)

#define	SPI_I2SCFG_CHLEN_POS	0U 
#define	SPI_I2SCFG_CHLEN_MSK	BIT(SPI_I2SCFG_CHLEN_POS)

/****************** Bit definition for SPI_I2SPR register ************************/

#define	SPI_I2SPR_EXTCKEN_POS	10U 
#define	SPI_I2SPR_EXTCKEN_MSK	BIT(SPI_I2SPR_EXTCKEN_POS)

#define	SPI_I2SPR_MCKOE_POS	9U 
#define	SPI_I2SPR_MCKOE_MSK	BIT(SPI_I2SPR_MCKOE_POS)

#define	SPI_I2SPR_ODD_POS	8U 
#define	SPI_I2SPR_ODD_MSK	BIT(SPI_I2SPR_ODD_POS)

#define	SPI_I2SPR_I2SDIV_POSS	0U 
#define	SPI_I2SPR_I2SDIV_POSE	7U 
#define	SPI_I2SPR_I2SDIV_MSK	BITS(SPI_I2SPR_I2SDIV_POSS,SPI_I2SPR_I2SDIV_POSE)

/****************** Bit definition for SPI_IER register ************************/

#define	SPI_IER_FREIE_POS	18U 
#define	SPI_IER_FREIE_MSK	BIT(SPI_IER_FREIE_POS)

#define	SPI_IER_MODFIE_POS	17U 
#define	SPI_IER_MODFIE_MSK	BIT(SPI_IER_MODFIE_POS)

#define	SPI_IER_CRCERRIE_POS	16U 
#define	SPI_IER_CRCERRIE_MSK	BIT(SPI_IER_CRCERRIE_POS)

#define	SPI_IER_RXTHIE_POS	12U 
#define	SPI_IER_RXTHIE_MSK	BIT(SPI_IER_RXTHIE_POS)

#define	SPI_IER_RXUDIE_POS	11U 
#define	SPI_IER_RXUDIE_MSK	BIT(SPI_IER_RXUDIE_POS)

#define	SPI_IER_RXOVIE_POS	10U 
#define	SPI_IER_RXOVIE_MSK	BIT(SPI_IER_RXOVIE_POS)

#define	SPI_IER_RXFIE_POS	9U 
#define	SPI_IER_RXFIE_MSK	BIT(SPI_IER_RXFIE_POS)

#define	SPI_IER_TXTHIE_POS	4U 
#define	SPI_IER_TXTHIE_MSK	BIT(SPI_IER_TXTHIE_POS)

#define	SPI_IER_TXUDIE_POS	3U 
#define	SPI_IER_TXUDIE_MSK	BIT(SPI_IER_TXUDIE_POS)

#define	SPI_IER_TXOVIE_POS	2U 
#define	SPI_IER_TXOVIE_MSK	BIT(SPI_IER_TXOVIE_POS)

#define	SPI_IER_TXEIE_POS	0U 
#define	SPI_IER_TXEIE_MSK	BIT(SPI_IER_TXEIE_POS)

/****************** Bit definition for SPI_IDR register ************************/

#define	SPI_IDR_FREID_POS	18U 
#define	SPI_IDR_FREID_MSK	BIT(SPI_IDR_FREID_POS)

#define	SPI_IDR_MODFID_POS	17U 
#define	SPI_IDR_MODFID_MSK	BIT(SPI_IDR_MODFID_POS)

#define	SPI_IDR_CRCERRID_POS	16U 
#define	SPI_IDR_CRCERRID_MSK	BIT(SPI_IDR_CRCERRID_POS)

#define	SPI_IDR_RXTHID_POS	12U 
#define	SPI_IDR_RXTHID_MSK	BIT(SPI_IDR_RXTHID_POS)

#define	SPI_IDR_RXUDID_POS	11U 
#define	SPI_IDR_RXUDID_MSK	BIT(SPI_IDR_RXUDID_POS)

#define	SPI_IDR_RXOVID_POS	10U 
#define	SPI_IDR_RXOVID_MSK	BIT(SPI_IDR_RXOVID_POS)

#define	SPI_IDR_RXFID_POS	9U 
#define	SPI_IDR_RXFID_MSK	BIT(SPI_IDR_RXFID_POS)

#define	SPI_IDR_TXTHID_POS	4U 
#define	SPI_IDR_TXTHID_MSK	BIT(SPI_IDR_TXTHID_POS)

#define	SPI_IDR_TXUDID_POS	3U 
#define	SPI_IDR_TXUDID_MSK	BIT(SPI_IDR_TXUDID_POS)

#define	SPI_IDR_TXOVID_POS	2U 
#define	SPI_IDR_TXOVID_MSK	BIT(SPI_IDR_TXOVID_POS)

#define	SPI_IDR_TXEID_POS	0U 
#define	SPI_IDR_TXEID_MSK	BIT(SPI_IDR_TXEID_POS)

/****************** Bit definition for SPI_IVS register ************************/

#define	SPI_IVS_FREIV_POS	18U 
#define	SPI_IVS_FREIV_MSK	BIT(SPI_IVS_FREIV_POS)

#define	SPI_IVS_MODFIV_POS	17U 
#define	SPI_IVS_MODFIV_MSK	BIT(SPI_IVS_MODFIV_POS)

#define	SPI_IVS_CRCERRIV_POS	16U 
#define	SPI_IVS_CRCERRIV_MSK	BIT(SPI_IVS_CRCERRIV_POS)

#define	SPI_IVS_RXTHIV_POS	12U 
#define	SPI_IVS_RXTHIV_MSK	BIT(SPI_IVS_RXTHIV_POS)

#define	SPI_IVS_RXUDIV_POS	11U 
#define	SPI_IVS_RXUDIV_MSK	BIT(SPI_IVS_RXUDIV_POS)

#define	SPI_IVS_RXOVIV_POS	10U 
#define	SPI_IVS_RXOVIV_MSK	BIT(SPI_IVS_RXOVIV_POS)

#define	SPI_IVS_RXFIV_POS	9U 
#define	SPI_IVS_RXFIV_MSK	BIT(SPI_IVS_RXFIV_POS)

#define	SPI_IVS_TXTHIV_POS	4U 
#define	SPI_IVS_TXTHIV_MSK	BIT(SPI_IVS_TXTHIV_POS)

#define	SPI_IVS_TXUDIV_POS	3U 
#define	SPI_IVS_TXUDIV_MSK	BIT(SPI_IVS_TXUDIV_POS)

#define	SPI_IVS_TXOVIV_POS	2U 
#define	SPI_IVS_TXOVIV_MSK	BIT(SPI_IVS_TXOVIV_POS)

#define	SPI_IVS_TXEIV_POS	0U 
#define	SPI_IVS_TXEIV_MSK	BIT(SPI_IVS_TXEIV_POS)

/****************** Bit definition for SPI_RIF register ************************/

#define	SPI_RIF_FRERI_POS	18U 
#define	SPI_RIF_FRERI_MSK	BIT(SPI_RIF_FRERI_POS)

#define	SPI_RIF_MODFRI_POS	17U 
#define	SPI_RIF_MODFRI_MSK	BIT(SPI_RIF_MODFRI_POS)

#define	SPI_RIF_CRCERRRI_POS	16U 
#define	SPI_RIF_CRCERRRI_MSK	BIT(SPI_RIF_CRCERRRI_POS)

#define	SPI_RIF_RXTHRI_POS	12U 
#define	SPI_RIF_RXTHRI_MSK	BIT(SPI_RIF_RXTHRI_POS)

#define	SPI_RIF_RXUDRI_POS	11U 
#define	SPI_RIF_RXUDRI_MSK	BIT(SPI_RIF_RXUDRI_POS)

#define	SPI_RIF_RXOVRI_POS	10U 
#define	SPI_RIF_RXOVRI_MSK	BIT(SPI_RIF_RXOVRI_POS)

#define	SPI_RIF_RXFRI_POS	9U 
#define	SPI_RIF_RXFRI_MSK	BIT(SPI_RIF_RXFRI_POS)

#define	SPI_RIF_TXTHRI_POS	4U 
#define	SPI_RIF_TXTHRI_MSK	BIT(SPI_RIF_TXTHRI_POS)

#define	SPI_RIF_TXUDRI_POS	3U 
#define	SPI_RIF_TXUDRI_MSK	BIT(SPI_RIF_TXUDRI_POS)

#define	SPI_RIF_TXOVRI_POS	2U 
#define	SPI_RIF_TXOVRI_MSK	BIT(SPI_RIF_TXOVRI_POS)

#define	SPI_RIF_TXERI_POS	0U 
#define	SPI_RIF_TXERI_MSK	BIT(SPI_RIF_TXERI_POS)

/****************** Bit definition for SPI_IFM register ************************/

#define	SPI_IFM_FREFM_POS	18U 
#define	SPI_IFM_FREFM_MSK	BIT(SPI_IFM_FREFM_POS)

#define	SPI_IFM_MODFFM_POS	17U 
#define	SPI_IFM_MODFFM_MSK	BIT(SPI_IFM_MODFFM_POS)

#define	SPI_IFM_CRCERRFM_POS	16U 
#define	SPI_IFM_CRCERRFM_MSK	BIT(SPI_IFM_CRCERRFM_POS)

#define	SPI_IFM_RXTHFM_POS	12U 
#define	SPI_IFM_RXTHFM_MSK	BIT(SPI_IFM_RXTHFM_POS)

#define	SPI_IFM_RXUDFM_POS	11U 
#define	SPI_IFM_RXUDFM_MSK	BIT(SPI_IFM_RXUDFM_POS)

#define	SPI_IFM_RXOVFM_POS	10U 
#define	SPI_IFM_RXOVFM_MSK	BIT(SPI_IFM_RXOVFM_POS)

#define	SPI_IFM_RXFFM_POS	9U 
#define	SPI_IFM_RXFFM_MSK	BIT(SPI_IFM_RXFFM_POS)

#define	SPI_IFM_TXTHFM_POS	4U 
#define	SPI_IFM_TXTHFM_MSK	BIT(SPI_IFM_TXTHFM_POS)

#define	SPI_IFM_TXUDFM_POS	3U 
#define	SPI_IFM_TXUDFM_MSK	BIT(SPI_IFM_TXUDFM_POS)

#define	SPI_IFM_TXOVFM_POS	2U 
#define	SPI_IFM_TXOVFM_MSK	BIT(SPI_IFM_TXOVFM_POS)

#define	SPI_IFM_TXEFM_POS	0U 
#define	SPI_IFM_TXEFM_MSK	BIT(SPI_IFM_TXEFM_POS)

/****************** Bit definition for SPI_ICR register ************************/

#define	SPI_ICR_FREIC_POS	18U 
#define	SPI_ICR_FREIC_MSK	BIT(SPI_ICR_FREIC_POS)

#define	SPI_ICR_MODFIC_POS	17U 
#define	SPI_ICR_MODFIC_MSK	BIT(SPI_ICR_MODFIC_POS)

#define	SPI_ICR_CRCERRIC_POS	16U 
#define	SPI_ICR_CRCERRIC_MSK	BIT(SPI_ICR_CRCERRIC_POS)

#define	SPI_ICR_RXTHIC_POS	12U 
#define	SPI_ICR_RXTHIC_MSK	BIT(SPI_ICR_RXTHIC_POS)

#define	SPI_ICR_RXUDIC_POS	11U 
#define	SPI_ICR_RXUDIC_MSK	BIT(SPI_ICR_RXUDIC_POS)

#define	SPI_ICR_RXOVIC_POS	10U 
#define	SPI_ICR_RXOVIC_MSK	BIT(SPI_ICR_RXOVIC_POS)

#define	SPI_ICR_RXFIC_POS	9U 
#define	SPI_ICR_RXFIC_MSK	BIT(SPI_ICR_RXFIC_POS)

#define	SPI_ICR_TXTHIC_POS	4U 
#define	SPI_ICR_TXTHIC_MSK	BIT(SPI_ICR_TXTHIC_POS)

#define	SPI_ICR_TXUDIC_POS	3U 
#define	SPI_ICR_TXUDIC_MSK	BIT(SPI_ICR_TXUDIC_POS)

#define	SPI_ICR_TXOVIC_POS	2U 
#define	SPI_ICR_TXOVIC_MSK	BIT(SPI_ICR_TXOVIC_POS)

#define	SPI_ICR_TXEIC_POS	0U 
#define	SPI_ICR_TXEIC_MSK	BIT(SPI_ICR_TXEIC_POS)

typedef struct
{
	__IO uint32_t CON1;
	__IO uint32_t CON2;
	__I uint32_t STAT;
	__IO uint32_t DATA;
	__IO uint32_t CRCPOLY;
	__I uint32_t RXCRC;
	__I uint32_t TXCRC;
	__IO uint32_t I2SCFG;
	__IO uint32_t I2SPR;
	__O uint32_t IER;
	__O uint32_t IDR;
	__I uint32_t IVS;
	__I uint32_t RIF;
	__I uint32_t IFM;
	__O uint32_t ICR;
} SPI_I2S_TypeDef;





#endif
