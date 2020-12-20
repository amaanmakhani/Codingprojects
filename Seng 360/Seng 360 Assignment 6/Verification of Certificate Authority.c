#include <stdio.h>
#include <openssl/bn.h>

#define NBITS 256

int main ()
{
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM *p = BN_new();
    BIGNUM *q = BN_new();
    BIGNUM *e = BN_new();
    BIGNUM* p_minus_one = BN_new();
    BIGNUM* q_minus_one = BN_new();
    BIGNUM *pmqm = BN_new();
    BIGNUM* one = BN_new();
    BIGNUM *d = BN_new();
    BIGNUM* enc = BN_new();
    BIGNUM* dec = BN_new();
    BIGNUM* privateKey = BN_new();
    BIGNUM* publicKey = BN_new();
    BIGNUM* mod = BN_new();
    BIGNUM* message = BN_new();
    BN_hex2bn(&p, "D753A40451F899A6164287E1C0D41573");
    BN_hex2bn(&q, "38c7dab1e327f0d9067c91ab6cd71a16");
    BN_hex2bn(&e, "10001");
    BN_dec2bn(&one, "1");
    BN_sub(p_minus_one, p, one);
    BN_sub(q_minus_one, q, one);
    BN_mul(pmqm, p_minus_one, q_minus_one, ctx);
    BN_mod_inverse(d, e, pmqm, ctx);
    BN_hex2bn(&sig, "35539711be68ceb9bf7dca53ff8e1beb36692be3be60ffbde72ac9e5078098bc");
    BN_hex2bn(&publicKey, "D753A40451F899A616484B6727AA9349D039ED0CB0B00087F1672886858C8E63DABCB14038E2D3F5ECA50518B83D3EC5991732EC188CFAF10CA6642185CB071034B052882B1F689BD2B18F12B0B3D2E7881F1FEF387754535F80793F2E1AAAA81E4B2B0DABB763B935B77D14BC594BDF514AD2A1E20CE29082876AAEEAD764D69855E8FDAF1A506C54BC11F2FD4AF29DBB7F0EF4D5BE8E16891255D8C07134EEF6DC2DECC48725868DD821E4B04D0C89DC392617DDF6D79485D80421709D6F6FFF5CBA19E145CB5657287E1C0D4157AAB7B827BBB1E4FA2AEF2123751AAD2D9B86358C9C77B573ADD8942DE4F30C9DEEC14E627E17C0719E2CDEF1F910281933");
    BN_hex2bn(&mod, "211f90a38c7dab1e327f0d9067c91ab6cd71a1619396d460481fa5779c58ec7f8d7f9bd0059f72a81e48d931b9b57f7ab5c9afc42a2e6cdf55bdd4e63981591a0bf42fb8d0c1e33b28b42defc86181058be6fe5e6bf8d86a513a78a3e1543d1d8deadeed659ff06ae04aa631392b3d9cb4247368c9f581ca931073fca6c50ded80a1266f7416000e72beeddc82931b2b1e3dfbbcddf8d112f1dc60c9286f9a5580646856be5d15b82d526b5a1354c8f928d1ef97a31ccc1a294e3de8071a5c00c631f755e6412dc022e2b77783f7237e38a8bf0c87c41d2b16fa121c21997711fb372e20c786b55330928e7996a0f29108b7b980a3807eee3d33d807dec6ae03");
    BN_hex2bn(&message, "4120746f702073656372657421");
    
    if(BN_mod_exp(dec, enc, sig, publicKey, ctx) == message){
        printf("CA signature verified");
    } else {
        printf("STOP!!! CA signature not verified!");
    }
    return 0;
}