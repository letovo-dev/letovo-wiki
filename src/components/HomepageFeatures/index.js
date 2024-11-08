import clsx from 'clsx';
import Heading from '@theme/Heading';
import styles from './styles.module.css';

const FeatureList = [
  {
    title: 'Че хочу, то и делаю',
    Svg: require('@site/static/img/1meme.jpg').default,
    description: (
      <>
        Любой формат!
      </>
    ),
  },
  {
    title: 'Возьму еще один мем впендюрю',
    Svg: require('@site/static/img/2meme.jpg').default,
    description: (
      <>
        И шо вы мне сделаете?
      </>
    ),
  },
  {
    title: 'Даже такая макака как я справилась',
    Svg: require('@site/static/img/3meme.jpg').default,
    description: (
      <>
        Кастомизации - полные штаны!
      </>
    ),
  },
];

function Feature({Svg, title, description}) {
  return (
    <div className={clsx('col col--4')}>
      <div className="text--center">
        <img src={Svg} className={styles.featureSvg} alt={title} />
        {/* <Svg className={styles.featureSvg} role="img" /> */}
      </div>
      <div className="text--center padding-horiz--md">
        <Heading as="h3">{title}</Heading>
        <p>{description}</p>
      </div>
    </div>
  );
}

export default function HomepageFeatures() {
  return (
    <section className={styles.features}>
      <div className="container">
        <div className="row">
          {FeatureList.map((props, idx) => (
            <Feature key={idx} {...props} />
          ))}
        </div>
      </div>
    </section>
  );
}
