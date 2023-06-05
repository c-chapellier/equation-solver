# examples/ventilation_design/gym_without_units.eqs.md

## Functions

$$abs(x) \rArr \mid x \mid$$

$$max(a, b) \rArr \frac{\left(a + b + abs\left(
a - b\right)\right)}{2}$$

$$sqrt(x) \rArr x^{0.5}$$

## Equations

$$T = 20$$

$$P = 101300$$

$$Rr_{inox} = 0.0003$$

$$\rho _{air} = 1.024$$

$$v_{air\_max\_ms} = 5$$

$$v_{air\_max\_mh} = 18000$$

$$prix_{mazout} = 0.85$$

$$prix_{kwh} = 0.0832$$

$$plafond_{z} = 2.77$$

$$salle_{y} = 10$$

$$salle_{x} = 9$$

$$salle_{aire} = salle_{x} \cdot salle_{y}$$

$$couloir_{y} = 4.2$$

$$couloir_{x} = 1.55$$

$$couloir_{aire} = couloir_{x} \cdot couloir_{y}$$

$$vestiaires_{y} = 4.2$$

$$vestiaire_{homme\_x} = 1.7$$

$$vestiaire_{homme\_aire} = vestiaire_{homme\_x} \cdot vestiaires_{y}$$

$$vestiaire_{femme\_x} = 2.1$$

$$vestiaire_{femme\_aire} = vestiaire_{femme\_x} \cdot vestiaires_{y}$$

$$toilettes_{hall\_y} = 0.9$$

$$toilettes_{hall\_x} = 1.55$$

$$toilettes_{hall\_aire} = toilettes_{hall\_x} \cdot toilettes_{hall\_y}$$

$$toilettes_{y} = 0.9$$

$$toilettes_{x} = 1.7$$

$$toilettes_{aire} = toilettes_{x} \cdot toilettes_{y}$$

$$porte_{z} = 2$$

$$porte_{x} = 73.5$$

$$porte_{aire} = porte_{x} \cdot porte_{z}$$

$$porte_{fenetre\_z} = 2.2$$

$$porte_{fenetre\_x} = 2.3$$

$$porte_{fenetre\_aire} = porte_{fenetre\_x} \cdot porte_{fenetre\_z}$$

$$aire_{totale} = salle_{aire} + couloir_{aire} + vestiaire_{homme\_aire} + vestiaire_{femme\_aire} + toilettes_{hall\_aire} + toilettes_{aire}$$

$$volume_{total} = aire_{totale} \cdot plafond_{z}$$

$$r_{petit\_coude} = 0.25$$

$$r_{grand\_coude} = 0.5$$

$$l_{conduites\_salle} = 11.5$$

$$l_{conduites\_sanitaires} = 20$$

$$l_{conduites\_ext} = 0.5$$

$$n_{athletes\_recommande} = \frac{\left(salle_{aire} + couloir_{aire}\right)}{10}$$

$$n_{athletes} = 10$$

$$met_{\mu scu} = 5.95$$

$$salle_{q\_min} = 22 \cdot n_{athletes} \cdot \left(\frac{met_{\mu scu}}{1.2}\right)$$

$$toilettes_{q\_min} = 25$$

$$vestiaire_{homme\_q\_min\_calc} = 5 \cdot vestiaire_{homme\_aire}$$

$$vestiaire_{homme\_q\_min} = max\left(
50, vestiaire_{homme\_q\_min\_calc}\right)$$

$$vestiaire_{femme\_q\_min\_calc} = 5 \cdot vestiaire_{femme\_aire}$$

$$vestiaire_{femme\_q\_min} = max\left(
50, vestiaire_{femme\_q\_min\_calc}\right)$$

$$q_{tot\_min} = salle_{q\_min} + 2 \cdot toilettes_{q\_min} + vestiaire_{homme\_q\_min} + vestiaire_{femme\_q\_min}$$

$$\pi  = 3.1416$$

$$d_{conduites\_salle\_min} = 2 \cdot sqrt\left(
\frac{salle_{q\_min}}{\left(v_{air\_max\_mh} \cdot \pi \right)}\right)$$

$$d_{conduites\_salle\_std} = 0.315$$

$$q_{conduites\_sanitaires} = 2 \cdot toilettes_{q\_min} + vestiaire_{homme\_q\_min} + vestiaire_{femme\_q\_min}$$

$$d_{conduites\_sanitaires\_min} = 2 \cdot sqrt\left(
\frac{q_{conduites\_sanitaires}}{\left(v_{air\_max\_mh} \cdot \pi \right)}\right)$$

$$d_{conduites\_sanitaires\_std} = 0.125$$

$$m_{dot\_air\_salle} = \frac{\rho _{air} \cdot salle_{q\_min}}{3600}$$

$$\Delta P_{conduites\_salle} = 5.379$$

$$m_{dot\_air\_sanitaires} = \frac{\rho _{air} \cdot q_{conduites\_sanitaires}}{3600}$$

$$\Delta P_{conduites\_sanitaires} = 21.09$$

$$m_{dot\_air\_ext} = m_{dot\_air\_salle} + m_{dot\_air\_sanitaires}$$

$$\Delta P_{conduites\_ext} = 0.4723$$

$$\Delta P_{petit\_coude} = 2.206$$

$$\Delta P_{grand\_coude} = 2.523$$

$$\Delta P_{entrees\_salle} = 6.4$$

$$\Delta P_{entrees\_sanitaires} = 6.4$$

$$\Delta P_{sorties} = 25.6$$

$$\Delta P_{salle} = \Delta P_{conduites\_salle} + 6 \cdot \Delta P_{grand\_coude} + 2 \cdot \Delta P_{entrees\_salle}$$

$$\Delta P_{sanitaires} = \Delta P_{conduites\_sanitaires} + 3 \cdot \Delta P_{petit\_coude} + 4 \cdot \Delta P_{entrees\_sanitaires}$$

$$\Delta P_{ext} = \Delta P_{conduites\_ext} + \Delta P_{sorties}$$

$$air_{chaleur\_volumique} = 0.34$$

$$Q_{dot\_ventilation} = \left(18 - 9\right) \cdot air_{chaleur\_volumique} \cdot q_{tot\_min}$$

$$rendement_{chauffage} = 0.7$$

$$equivalent_{combustible} = \frac{Q_{dot\_ventilation}}{\left(1000 \cdot rendement_{chauffage} \cdot 10\right)}$$

$$P_{ventilation} = 450$$

$$C_{horaire\_ventilaion} = prix_{kwh} \cdot \left(\frac{450}{1000}\right)$$

$$C_{horaire\_chauffage} = prix_{mazout} \cdot equivalent_{combustible}$$

$$C_{tot} = C_{horaire\_ventilaion} + C_{horaire\_chauffage}$$

$$roi = \frac{\left(\frac{6250}{\left(0.8 \cdot C_{horaire\_chauffage} - 0.3 \cdot prix_{kwh}\right)}\right)}{4}$$

## Solution

$$T = 20$$

$$P = 101300$$

$$Rr_{inox} = 0.0003$$

$$\rho _{air} = 1.024$$

$$v_{air\_max\_ms} = 5$$

$$v_{air\_max\_mh} = 18000$$

$$prix_{mazout} = 0.85$$

$$prix_{kwh} = 0.0832$$

$$plafond_{z} = 2.77$$

$$salle_{y} = 10$$

$$salle_{x} = 9$$

$$salle_{aire} = 90$$

$$couloir_{y} = 4.2$$

$$couloir_{x} = 1.55$$

$$couloir_{aire} = 6.51$$

$$vestiaires_{y} = 4.2$$

$$vestiaire_{homme\_x} = 1.7$$

$$vestiaire_{homme\_aire} = 7.14$$

$$vestiaire_{femme\_x} = 2.1$$

$$vestiaire_{femme\_aire} = 8.82$$

$$toilettes_{hall\_y} = 0.9$$

$$toilettes_{hall\_x} = 1.55$$

$$toilettes_{hall\_aire} = 1.395$$

$$toilettes_{y} = 0.9$$

$$toilettes_{x} = 1.7$$

$$toilettes_{aire} = 1.53$$

$$porte_{z} = 2$$

$$porte_{x} = 73.5$$

$$porte_{aire} = 147$$

$$porte_{fenetre\_z} = 2.2$$

$$porte_{fenetre\_x} = 2.3$$

$$porte_{fenetre\_aire} = 5.06$$

$$aire_{totale} = 115.395$$

$$volume_{total} = 319.64415$$

$$r_{petit\_coude} = 0.25$$

$$r_{grand\_coude} = 0.5$$

$$l_{conduites\_salle} = 11.5$$

$$l_{conduites\_sanitaires} = 20$$

$$l_{conduites\_ext} = 0.5$$

$$n_{athletes\_recommande} = 9.651$$

$$n_{athletes} = 10$$

$$met_{\mu scu} = 5.95$$

$$salle_{q\_min} = 1090.833333$$

$$toilettes_{q\_min} = 25$$

$$vestiaire_{homme\_q\_min\_calc} = 35.7$$

$$vestiaire_{homme\_q\_min} = 50$$

$$vestiaire_{femme\_q\_min\_calc} = 44.1$$

$$vestiaire_{femme\_q\_min} = 50$$

$$q_{tot\_min} = 1240.833333$$

$$\pi  = 3.1416$$

$$d_{conduites\_salle\_min} = 0.277778$$

$$d_{conduites\_salle\_std} = 0.315$$

$$q_{conduites\_sanitaires} = 150$$

$$d_{conduites\_sanitaires\_min} = 0.103006$$

$$d_{conduites\_sanitaires\_std} = 0.125$$

$$m_{dot\_air\_salle} = 0.310281$$

$$\Delta P_{conduites\_salle} = 5.379$$

$$m_{dot\_air\_sanitaires} = 0.042667$$

$$\Delta P_{conduites\_sanitaires} = 21.09$$

$$m_{dot\_air\_ext} = 0.352948$$

$$\Delta P_{conduites\_ext} = 0.4723$$

$$\Delta P_{petit\_coude} = 2.206$$

$$\Delta P_{grand\_coude} = 2.523$$

$$\Delta P_{entrees\_salle} = 6.4$$

$$\Delta P_{entrees\_sanitaires} = 6.4$$

$$\Delta P_{sorties} = 25.6$$

$$\Delta P_{salle} = 33.317$$

$$\Delta P_{sanitaires} = 53.308$$

$$\Delta P_{ext} = 26.0723$$

$$air_{chaleur\_volumique} = 0.34$$

$$Q_{dot\_ventilation} = 3796.95$$

$$rendement_{chauffage} = 0.7$$

$$equivalent_{combustible} = 0.542421$$

$$P_{ventilation} = 450$$

$$C_{horaire\_ventilaion} = 0.03744$$

$$C_{horaire\_chauffage} = 0.461058$$

$$C_{tot} = 0.498498$$

$$roi = 4543.649359$$

